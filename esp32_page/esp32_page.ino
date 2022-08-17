#include<WiFi.h>
#include<SPIFFS.h>
#include<ArduinoJson.h>
#include<ESPAsyncWebServer.h>

#define WIFIJSON "/wifi.json"
#define SSIDJSON "/ssid.json"

AsyncWebServer server(80);

typedef struct data1{
  int setting;
  char ssid[20];
  char password[20];
} WIFI;

typedef struct data2{
  char ssid[20];
  int rssi;
} SSIDINFO;

WIFI wifi={0,"",""};
SSIDINFO ssidInfo={"",0};

int wifiCounts=0;

/*const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";*/
const char * host="blanka";
String wifiList="";

void parseWifiJson()
{
  File file=SPIFFS.open(WIFIJSON,"r");

  if(file&&file.size())
  {
    Serial.println(file.size());
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, file);

    if (error)
    {
      Serial.printf("deserializeJson() %s failed: ",WIFIJSON);
      Serial.println(error.c_str());
      return;
    }

    /*int setting = doc["setting"]; // 0
    const char* ssid = doc["ssid"]; // "111111111111111111111111111111111111111111111111"
    const char* password = doc["password"]; // "111111111111111111111111111111111111111111111111"
    Serial.printf("setting:%d,ssid:%s,password:%s\n",setting,ssid,password);*/
    wifi.setting = doc["setting"]; // 0
    strcpy(wifi.ssid,doc["ssid"]); // "111111111111111111111111111111111111111111111111"
    strcpy(wifi.password,doc["password"]); // "111111111111111111111111111111111111111111111111"
    Serial.printf("2setting:%d,ssid:%s,password:%s\n",wifi.setting,wifi.ssid,wifi.password);
    file.close();
  }
  else
  {
    Serial.printf("open %s failed\n",WIFIJSON);
  }
}

void scanWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("scan start");
  wifiCounts=WiFi.scanNetworks();
  Serial.println("scan done");
  if(wifiCounts==0)
  {
    Serial.println("no networks found");
  } 
  else{
    Serial.print(wifiCounts);
    Serial.println(" networks found");
    //File file=SPIFFS.open(SSIDJSON,"w");
    wifiList="";
    wifiList+="{\"ssids\":[";
    for(int i=0;i<wifiCounts;++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      wifiList+="{\"ssid\":\""+WiFi.SSID(i)+"\",";
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      wifiList+="\"rssi\":"+String(WiFi.RSSI(i))+"}";
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
      if(i!=wifiCounts-1)
      {
         wifiList+=",";
      }
    }
    wifiList+="]}";
  }
  Serial.println(wifiList);
}

void wifiAPMode()
{
  WiFi.softAP("ESP32_AP","12345678");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("home.html");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/js",SPIFFS,"/www/js/");
  server.serveStatic("/css",SPIFFS,"/www/css/");
  
  server.on("/wifiCounts",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",String(wifiCounts));
    });

  server.on("/wifiList",HTTP_GET,[](AsyncWebServerRequest * req){      
    /*AsyncResponseStream *response = req->beginResponseStream("application/json");
    DynamicJsonDocument json(1024);
    json["ssid"] = ssidInfo.ssid;
    json["rssi"] = ssidInfo.rssi;
    serializeJson(json, *response);
    req->send(response);*/
    req->send(200,"text/plain",wifiList);    
    });
    
  server.on("/wifiConnect",HTTP_POST,[](AsyncWebServerRequest * req){   
    Serial.println("/wifiConnect");   
    int paramsNr = req->params(); // number of params (e.g., 1)
    Serial.println(paramsNr);
    Serial.println();
    AsyncWebParameter * s = req->getParam(0); //ssid
    AsyncWebParameter * p = req->getParam(1); //password
    Serial.println(s->value());
    Serial.println(p->value());
    strcpy(wifi.ssid,s->value().c_str());
    strcpy(wifi.password,p->value().c_str());
    wifiConnect(wifi.ssid,wifi.password);
    req->send(200,"text/plain",String("OK"));    
    });
   
  server.begin();
}

void wifiConnect(char *ssid, char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.hostname("OTA_WEBSITE");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
}

void setup()
{
  Serial.begin(115200);


  if(!SPIFFS.begin(true))
  {
    Serial.println("ESP32 SPIFFS is broken");
    return;
  }

  delay(1000);
  parseWifiJson();
  if(wifi.setting==0)
  {
    scanWifi();
    wifiAPMode();
  }
  else
  {
    wifiConnect(wifi.ssid,wifi.password);
  }
  /*server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/js",SPIFFS,"/www/js/");
  server.serveStatic("/css",SPIFFS,"/www/css/");
  server.begin();*/
}

void loop()
{
  // put your main code here, to run repeatedly:

}
