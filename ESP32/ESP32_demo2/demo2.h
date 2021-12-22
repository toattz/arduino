//#include<WiFi.h>
#include <ESPAsync_WiFiManager.h> 
#include<SPIFFS.h>
#include"json.h"
//#include<ESPAsyncWebServer.h>
#include<HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include<SD.h>

//ADC write sample 10bits
#define BITS 10 

//LED pin define
#define GREEN_LED 2
#define YELLOW_LED 4

/*The information is available on SDK level. If you include #include "user_interface.h", you can use function wifi_station_get_connect_status(). It returns one of:

STATION_GOT_IP
STATION_NO_AP_FOUND
STATION_CONNECT_FAIL
STATION_WRONG_PASSWORD
STATION_IDLE
The WiFi.status() function uses function wifi_station_get_connect_status(), but returns WL_CONNECT_FAILED for STATION_WRONG_PASSWORD.*/

enum Status{
  INITIALIZATION,
  WIFI_AP_MODE,
  WIFI_STA_MODE 
};

enum Storage{
  STORAGE_INIT,
  SPIFFS_FAIL,
  SD_OK,
  SD_FAIL,
  SD_READ,
  SD_WRITE
}; 

String ssidList="";

const char *st[]={
  "initialization",
  "wifi ap mode",
  "wifi sta mode"
};

bool turnOnSTA=0;
byte conut=0;
String serverAddr="http://192.168.1.116";

AsyncWebServer server(80);
DNSServer dnsServer;
//Status status=WIFI_NOT_SCANNED;
Status status=INITIALIZATION;
Storage storage=STORAGE_INIT;
WiFiClient client;
HTTPClient http;
LiquidCrystal_I2C lcd(0x27,20,4);
/*SemaphoreHandle_t xMutex=xSemaphoreCreateMutex();
TaskHandle_t handleLCD;
TaskHandle_t handleGreenLED;
TaskHandle_t handleAlive;*/



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
    //status=WIFI_NOT_SCANNED;
  }
  else if(initStatus==0)
  {
    Serial.println("getInit()==0,Not Initialized");
    setInit();  
    //status=WIFI_NOT_SCANNED;
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
      setReset(0);
      Serial.println("reset wifi config");
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

void taskStatus(void *pvParam)
{
  while(1)
  {
    if(WiFi.getMode()==WIFI_OFF)
    {
      #if DEBUG    
        Serial.println("WIFI_OFF");    
      #endif
    }
    else if(WiFi.getMode()==WIFI_STA)
    {
      #if DEBUG
        Serial.println("WIFI_STA");
      #endif
      status=WIFI_STA_MODE;
    }
    else if(WiFi.getMode()==WIFI_AP)
    {
      #if DEBUG
        Serial.println("WIFI_AP");
      #endif
      status=WIFI_AP_MODE;
    }
    else if(WiFi.getMode()==WIFI_AP_STA)
    {
      #if DEBUG
        Serial.println("WIFI_AP_STA");  
      #endif
      if(WiFi.status()==WL_CONNECTED)
      {
        status=WIFI_STA_MODE;
      }
      else
      {
        status=WIFI_AP_MODE;
      }
    }
    else
    {
      // do nothing
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
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
     if(storage==SPIFFS_FAIL)
    {
      lcd.setCursor(0,1);
      lcd.print("SPIFFS fail");
    }
    
    if(storage==SD_FAIL)
    {
      lcd.setCursor(0,1);
      lcd.print("SD card fail");
    }
    //xSemaphoreTake(xMutex,portMAX_DELAY);
    //if((lcdStatus!=status)&&(status!=WIFI_CONNECTING))
    if(lcdStatus!=status)
    {
      lcdStatus=status;
      lcd.clear();

      if(status==WIFI_STA_MODE)
      {
        delay(1500); //delay to wait get correct status then get correct IP address
        lcd.setCursor(0,0);
        lcd.printf("STA mode info");
        lcd.setCursor(0,1);
        lcd.printf("IP:%s",WiFi.localIP().toString().c_str());
        lcd.setCursor(0,2);
        lcd.printf("Free Size:%uMB",(SD.totalBytes()-SD.usedBytes())/(1024*1024));
        lcd.setCursor(0,3);
        lcd.printf("UUID:%s",uuid);
      }
      else if(status==WIFI_AP_MODE)
      {
        lcd.setCursor(0,0);
        lcd.printf("SOFT AP mode info");
        lcd.setCursor(0,1);
        lcd.printf("IP:%s",WiFi.softAPIP().toString().c_str());
        lcd.setCursor(0,2);
        lcd.printf("ssid:%s",ssid);
        lcd.setCursor(0,3);
        lcd.printf("UUID:%s",uuid);
      }
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
    if(status==WIFI_STA_MODE)
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

void taskYellowLED(void *pvParam)
{
  pinMode(YELLOW_LED,OUTPUT); 
  while(1)
  {
    if(storage==SD_FAIL||storage==SPIFFS_FAIL)
    {
      digitalWrite(YELLOW_LED,HIGH);
    }
    else if(storage==SD_OK)
    {
      digitalWrite(YELLOW_LED,LOW);
    }
    else
    {
      digitalWrite(YELLOW_LED,!digitalRead(YELLOW_LED));
    }
    vTaskDelay(300/portTICK_PERIOD_MS);
  }
}

void taskAlive(void *pvParam)
{
  while(1)
  {
    if((getOnboard()==1)&&(status==WIFI_STA_MODE))
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
