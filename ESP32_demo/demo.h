#include<BluetoothSerial.h>
#include<WiFi.h>
#include<SPIFFS.h>
#include"json.h"
#include<ESPAsyncWebServer.h>
#include<HTTPClient.h>
#include <LiquidCrystal_I2C.h>
//#include<SD.h>

//ADC write sample 10bits
#define BITS 10 

//LED pin define
#define GREEN_LED 2
#define YELLOW_LED 4

//wifi connect limit
#define WIFI_CONNECT_TIMEOUT_COUNT 30

/*The information is available on SDK level. If you include #include "user_interface.h", you can use function wifi_station_get_connect_status(). It returns one of:

STATION_GOT_IP
STATION_NO_AP_FOUND
STATION_CONNECT_FAIL
STATION_WRONG_PASSWORD
STATION_IDLE
The WiFi.status() function uses function wifi_station_get_connect_status(), but returns WL_CONNECT_FAILED for STATION_WRONG_PASSWORD.*/

enum Status{
  INITIALIZATION,
  WAIT_BLC_CONNECTION,
  WIFI_DISCONNECTED,
  WIFI_NOT_SCANNED,
  WIFI_SETUP_SSID,
  WIFI_SETUP_SSID_TO_WIFI_SETUP_PASSWORD,
  WIFI_SETUP_PASSWORD,
  WIFI_SETUP,
  WIFI_CONNECTING,
  WIFI_CONNECTED,
  WIFI_CONNECT_FAIL,
  WEB_UP
};

enum Storage{
  STORAGE_INIT,
  SPIFFS_FAIL,
  STORAGE_OK,
  STORAGE_FAIL,
  STORAGE_READ,
  STORAGE_WRITE
}; 

String ssidList="";

const char *st[]={
  "initialization",
  "wait blc connection",
  "wifi disconnected",
  "wifi not scanned",
  "wifi setup ssid",
  "wifi setup ssid to wifi setup password",
  "wifi setup password",
  "wifi setup",
  "wifi connecting",
  "wifi connected",
  "wifi connect fail",
  "web up"
  };
char *pin="0000";
char val;
byte conut=0;
String serverAddr="http://192.168.1.116";

BluetoothSerial BT;
AsyncWebServer server(80);
//Status status=WIFI_NOT_SCANNED;
Status status=INITIALIZATION;
Storage storage=STORAGE_INIT;
WiFiClient client;
HTTPClient http;
LiquidCrystal_I2C lcd(0x27,20,4);
//SemaphoreHandle_t xMutex=xSemaphoreCreateMutex();
TaskHandle_t handleLCD;
TaskHandle_t handleGreenLED;
TaskHandle_t handleAlive;



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
    status=WIFI_NOT_SCANNED;
  }
  else if(initStatus==0)
  {
    Serial.println("getInit()==0,Not Initialized");
    setInit();  
    status=WIFI_NOT_SCANNED;
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
  StaticJsonDocument<200> writeDoc;
  writeDoc["mac"]=WiFi.macAddress();
  writeDoc["uuid"]=uuid;
  writeDoc["status"]="online";
  String requestBody;
  serializeJson(writeDoc, requestBody);
  int httpCode=http.POST(requestBody);

  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("postOnboard() %s\n",payload.c_str());
    StaticJsonDocument<50> readDoc;
    DeserializationError error = deserializeJson(readDoc,payload);

    if (error) 
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    const char* temp = readDoc["onboard"]; // "success"
    Serial.println(temp);
    if(String(temp)=="success")
    {
      Serial.println("onboard success"); 
      setOnboard(1);
    }    
    else
    {
      Serial.println("onboard fail"); 
    }
  }
  else
  {
    Serial.println("postOnboard() error request");
  }
  http.end();  
  getJson();
}

void postDiscard()
{
  String url=serverAddr+"/discard";
  Serial.printf("url:%s\n",url.c_str());
  http.begin(client,url);
  http.addHeader("Content-Type","application/json");
  StaticJsonDocument<200> writeDoc;
  writeDoc["uuid"]=uuid;
  writeDoc["status"]="discarded";
  String requestBody;
  serializeJson(writeDoc, requestBody);
  int httpCode=http.POST(requestBody);

  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("postDiscard() %s\n",payload.c_str());
    StaticJsonDocument<50> readDoc;
    DeserializationError error = deserializeJson(readDoc,payload);

    if (error) 
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    const char* temp = readDoc["discard"]; // "success"
    Serial.println(temp);
    if(String(temp)=="success")
    {
      Serial.println("discard success"); 
      setOnboard(0);
      setConnected(0);
      setUUID();
      WiFi.disconnect(true);
      status=WIFI_DISCONNECTED;
    }    
    else
    {
      Serial.println("discard fail"); 
    }
  }
  else
  {
    Serial.println("postDiscard() error request");
  }
  http.end();  
  getJson();  
}

void taskLCD(void *pvParam)
{
  byte lcdStatus=INITIALIZATION;
  Wire.begin();
  Wire.setClock(5000);//set wire clock to prevent LCD garbled 5kMhz
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initializtion...");
  delay(1000);
  while(1)
  {
    if(storage==STORAGE_FAIL)
    {
      lcd.setCursor(0,1);
      lcd.print("Storage fail");
    }
    //xSemaphoreTake(xMutex,portMAX_DELAY);
    if((lcdStatus!=status)&&(status!=WIFI_CONNECTING))
    //if(lcdStatus!=status)
    {
      lcdStatus=status;
      lcd.clear();
      lcd.setCursor(0,0);
      if(status==WIFI_CONNECTED)
      {
        lcd.printf("IP:%s",WiFi.localIP().toString().c_str());
      }
      else
      {
        lcd.print("No wifi connection");
      }
      lcd.setCursor(0,1);
      lcd.printf("onboard:%d",getOnboard());
      lcd.setCursor(0,2);
      lcd.printf("UUID:%s",uuid);
      lcd.setCursor(0,3);
      lcd.print(st[status]);
    }
    //xSemaphoreGive(xMutex);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void taskGreenLED(void *pvParam)
{
  pinMode(GREEN_LED,OUTPUT);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0,5000,BITS);
  ledcAttachPin(GREEN_LED,0);
  int lumen=0;
  bool up=true;
  while(1)
  {
    //xSemaphoreTake(xMutex,portMAX_DELAY);
    ledcWrite(0,lumen);
    if(status==WIFI_CONNECTED)
    {
      lumen=1024;
    }
    else
    {
      if(up)
      {
        lumen+=100;
        if(lumen>1024)
        {
          lumen=1023;
          up=false;
        }
      }
      else
      {
        lumen-=100;
        if(lumen<0)
        {
          lumen=1;
          up=true;
        }
      }
    }
    //xSemaphoreGive(xMutex);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

/*void taskYellowLED(void *pvParam)
{
  pinMode(YELLOW_LED,OUTPUT); 
  while(1)
  {
    if(storage==STORAGE_FAIL)
    {
      digitalWrite(YELLOW_LED,HIGH);
    }
    else if(storage==STORAGE_OK)
    {
      digitalWrite(YELLOW_LED,LOW);
    }
    else
    {
      digitalWrite(YELLOW_LED,!digitalRead(YELLOW_LED));
    }
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}*/
void taskAlive(void *pvParam)
{
  while(1)
  {
    if((getOnboard()==1)&&(status==WIFI_CONNECTED))
    {
      //Serial.println("Send alive"); 
      String url=serverAddr+"/alive";
      //Serial.printf("url:%s\n",url.c_str());
      http.begin(client,url);
      http.addHeader("Content-Type","application/json");
      StaticJsonDocument<48> writeDoc;
      writeDoc["uuid"]=uuid;
      String requestBody;
      serializeJson(writeDoc, requestBody);
      int httpCode=http.POST(requestBody);
    }
    vTaskDelay(10000/portTICK_PERIOD_MS);
  }
}

void taskGetFreeHeapWhenRunning(void *pvParam)
{
  while(1)
  {
    Serial.printf("running tasks heap size:%u bytes\n",ESP.getFreeHeap());
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}
