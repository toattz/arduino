#include<BluetoothSerial.h>
#include<WiFi.h>
#include<SPIFFS.h>
#include"json.h"
#include<ESPAsyncWebServer.h>
#include<HTTPClient.h>

/*The information is available on SDK level. If you include #include "user_interface.h", you can use function wifi_station_get_connect_status(). It returns one of:

STATION_GOT_IP
STATION_NO_AP_FOUND
STATION_CONNECT_FAIL
STATION_WRONG_PASSWORD
STATION_IDLE
The WiFi.status() function uses function wifi_station_get_connect_status(), but returns WL_CONNECT_FAILED for STATION_WRONG_PASSWORD.*/

enum Status{
  WAIT_BLC_CONNECTION,
  WIFI_DISCONNECTED,
  WIFI_NOT_SCANNED,
  WIFI_SETUP_SSID,
  WIFI_SETUP_SSID_TO_WIFI_SETUP_PASSWORD,
  WIFI_SETUP_PASSWORD,
  WIFI_SETUP,
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WEB_UP
};

String ssidList="";

const char *st[]={
  "wait blc connection",
  "wifi disconnected",
  "wifi not scanned",
  "wifi setup ssid",
  "wifi setup ssid to wifi setup password",
  "wifi setup password",
  "wifi setup",
  "wifi connecting",
  "wifi connected"
  };
char *pin="0000";
char val;
byte conut=0;
String serverAddr="http://192.168.1.116";

BluetoothSerial BT;
AsyncWebServer server(80);
Status status=WIFI_NOT_SCANNED;
WiFiClient client;
HTTPClient http;

void printFile(const char *filename)  //print JSON file
{
  // Open file for reading
  File file = SPIFFS.open(filename);
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }
 
  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();
 
  // Close the file
  file.close();
}

void checkConfig()
{ 
  byte initStatus=getInit();

  if(initStatus==1)
  {
    Serial.println("getInit()==1,Initialized");
    getUUID();
  }
  else if(initStatus==0)
  {
    Serial.println("getInit()==0,Not Initialized");
    setInit();  
  }
  else if(initStatus==2)
  {
    Serial.println("checkConfig() deserializeJson() failed:");
  }
  else
  {
    Serial.println("checkConfig() No config.json");
  }
}

void scanWIFI()
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.hostname(String("ESP32 ")+uuid);
  //WiFi.disconnect();
  delay(100);
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if(n==0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.printf("%d networks found\n",n);
    BT.printf("%d networks found\n",n);
    StaticJsonDocument<1024> doc;
    JsonArray devices = doc.createNestedArray("devices");
    for (byte i = 0; i < n; ++i) 
    {
      if(i>5)
      {
        continue;
      }
      else
      {
        JsonObject device = devices.createNestedObject();
        device["id"]=i+1;
        device["ssid"]=WiFi.SSID(i);
        device["rssi"]=WiFi.RSSI(i);
        Serial.printf("%d. %s(%d)\n",i+1,WiFi.SSID(i).c_str(),WiFi.RSSI(i));
        BT.printf("%d. %s(%d)\n",i+1,WiFi.SSID(i).c_str(),WiFi.RSSI(i));
        delay(10);
      }
    }
    ssidList=""; 
    serializeJson(doc, ssidList);
    Serial.println(ssidList.c_str());
  }
  status=WIFI_DISCONNECTED;
}

void listWIFI()
{
  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, ssidList);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  for (JsonObject device : doc["devices"].as<JsonArray>())
  {
    int device_id = device["id"]; // 1, 2, 3, 4, 5, 6
    const char* device_ssid = device["ssid"]; // "DIRECT-ab-HP M28 LaserJet", "ZYXEL_1F", ...
    int device_rssi = device["rssi"]; // -42, -54, -68, -90, -91, -92
    Serial.printf("id:%d ssid:%s rssi:%d\n",device_id,device_ssid,device_rssi);
    BT.printf("id:%d ssid:%s rssi:%d\n",device_id,device_ssid,device_rssi);
  }
}

void selectSSID(int id)
{
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, ssidList);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  for (JsonObject device : doc["devices"].as<JsonArray>())
  {
    int device_id = device["id"]; // 1, 2, 3, 4, 5, 6
    const char* device_ssid = device["ssid"]; // "DIRECT-ab-HP M28 LaserJet", "ZYXEL_1F", ...
    if(device_id==id)
    {
      strcpy(ssid,device_ssid);   
      Serial.printf("id:%d ssid:%s is selected\n",id,ssid);
      BT.printf("id:%d ssid:%s is selected\n",id,ssid);
    }
  }  
}

void getTest()
{
  String url=serverAddr+"/test";
  Serial.printf("url:%s\n",url.c_str());
  http.begin(url);
  int httpCode=http.GET();
  Serial.printf("httpCode:%d\n",httpCode);
  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("getTest() %s\n",payload.c_str());
  }
  else
  {
    Serial.println("getTest() error request");
  }
  http.end();
}

void postTest()
{
  String url=serverAddr+"/test?t=1&e=2&k=3";
  Serial.printf("url:%s\n",url.c_str());
  http.begin(client,url);
  http.addHeader("Content-Type","application/json");
  StaticJsonDocument<200> doc;
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);
  String requestBody;
  serializeJson(doc, requestBody);
  int httpCode=http.POST(requestBody);

  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("postTest() %s\n",payload.c_str());
  }
  else
  {
    Serial.println("postTest() error request");
  }
  http.end();  
}

void postOnboard()
{
  String url=serverAddr+"/onboard";
  Serial.printf("url:%s\n",url.c_str());
  http.begin(client,url);
  http.addHeader("Content-Type","application/json");
  StaticJsonDocument<200> doc;
  doc["mac"]=WiFi.macAddress();
  doc["uuid"]= uuid;
  String requestBody;
  serializeJson(doc, requestBody);
  int httpCode=http.POST(requestBody);

  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("postTest() %s\n",payload.c_str());
  }
  else
  {
    Serial.println("postTest() error request");
  }
  http.end();  
}
