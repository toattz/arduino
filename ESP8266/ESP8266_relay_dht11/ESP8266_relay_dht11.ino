#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<DHT.h>
#include<FS.h>

#define RELAYPIN D1
#define DHTPIN D4
#define DHTTYPE DHT11

//const char * ssid="ZYXEL_1F";
const char * ssid="TP-LINK_470A9A";
const char * password="C1470A9A";
volatile bool manual=false;
volatile byte topTemp=30,lowTemp=20;
volatile float temp=30,humi=60;

AsyncWebServer server(80);
DHT dht(DHTPIN,DHTTYPE);

void readDHT()
{
  Serial.println("readDHT");
  Serial.println(millis());
  temp=dht.readTemperature();
  humi=dht.readHumidity();
  if(manual==false)
  {
    Serial.println("manual is false");
    if(temp<lowTemp)
    {
      Serial.println("temp<lowTemp");
      Serial.printf("temp:%f<lowTemp:%d\n",temp,lowTemp);
      digitalWrite(RELAYPIN,LOW);
    }
    else
    {
      Serial.println("temp>=lowTemp");
      Serial.printf("temp:%f>=lowTemp:%d\n",temp,lowTemp);
    }

    if(temp>topTemp)
    {
      Serial.println("temp>topTemp");
      Serial.printf("temp:%f>topTemp:%d\n",temp,topTemp);
      digitalWrite(RELAYPIN,HIGH);
    }
    else
    {
      Serial.println("temp<=topTemp");
      Serial.printf("temp:%f<=topTemp:%d\n",temp,topTemp);
    }
  }
  else
  {
    Serial.println("manual is true");
  }
  //Serial.println(String("Temperature:")+temp);
  //Serial.println(String("Humidity:")+humi);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("start system");
  dht.begin();
  pinMode(RELAYPIN,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(D5,OUTPUT);
  digitalWrite(D5,HIGH);
  
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.hostname("心心用");
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    delay(500);
  }

  Serial.printf("\nIP Address:%s\n",WiFi.localIP().toString().c_str());

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");

  server.on("/status",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("status");
    String res="{\"temperature\":";
    res+=String(temp)+',';
    res+="\"temperatureTop\":"+String(topTemp)+",";
    res+="\"temperatureLow\":"+String(lowTemp)+",";
    res+="\"status\":";
    if(digitalRead(RELAYPIN)==HIGH)
    {
      res+="0,"; 
    }
    else
    {
      res+="1,";
    }
    //res+="\"millis\":"+String(millis())+",";
    if(manual==true)
    {
      res+="\"manual\":1}";
    }
    else
    {
      res+="\"manual\":0}";
    }
    Serial.println(res);
    req->send(200,"json/application",res);
    });

  server.on("/manual",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("manual");
    manual=!manual;
    if(manual==true)
    {
      Serial.println("true");
      req->send(200,"text/plain",String(1));
    }
    else
    {
      Serial.println("false");
      req->send(200,"text/plain",String(0));
    }
    });

  server.on("/heating",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("heating");
    digitalWrite(RELAYPIN,!digitalRead(RELAYPIN));
    if(digitalRead(RELAYPIN)==LOW)
    {
      Serial.println("low");
      req->send(200,"text/plain",String(1));
    }
    else
    {
      Serial.println("high");
      req->send(200,"text/plain",String(0));
    }
    });

  server.on("/temperature",HTTP_GET,[](AsyncWebServerRequest * req)
  {
    Serial.println("temperature"); 
    if(req->hasParam("temperatureTop"))
    {
      AsyncWebParameter * p=req->getParam("temperatureTop");
      topTemp=p->value().toInt();
      Serial.println(topTemp);  
    }

    if(req->hasParam("temperatureLow"))
    {
      AsyncWebParameter * p=req->getParam("temperatureLow");
      lowTemp=p->value().toInt();
      Serial.println(lowTemp);  
    }    
    req->send(200);
  });

  /*server.on("/hint",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("hint");
    for(byte i=0;i<6;i++)
    {
      digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
      delay(500);
    }
    req->send(200);
    });*/
  server.begin();
  Serial.println("Web server is started");
}

void loop()
{
  readDHT();
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  delay(3000);
}
