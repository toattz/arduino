#include "demo2.h"

/*const String serverAddr="http://192.168.1.116";
WiFiClient client;
HTTPClient http;*/


void OnWiFiEvent(WiFiEvent_t event)
{
  switch (event) {
 
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("ESP32 Connected to WiFi Network");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("ESP32 soft AP started");
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Station connected to ESP32 soft AP");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("Station disconnected from ESP32 soft AP");
      break;
    default: break;
  }
}

void setup() 
{
  Serial.begin(115200);

  //create FREERTOS task
  xTaskCreate(taskStatus,"taskStatus",2048,NULL,1,NULL);
  xTaskCreate(taskLCD,"taskLCD",2500,NULL,1,NULL);
  xTaskCreate(taskGreenLED,"taskGreenLED",1000,NULL,1,NULL);
  xTaskCreate(taskYellowLED,"taskYellowLED",1000,NULL,1,NULL);

  //initailize SPIFFS
  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    storage=SPIFFS_FAIL;
    return;
  }

  //initialize SD card
  if(!SD.begin())
  {
    Serial.println("Mount SD fail");
    storage=SD_FAIL;
    return;
  }
  else
  {
    Serial.println("Mount SD success");
    storage=SD_OK;
  }

  //to get or generate UUID and initialized
  checkConfig();
  
  ESPAsync_WiFiManager ESPAsync_wifiManager(&server, &dnsServer, "Async_AutoConnect");
  if(getReset()==0)
  {
    ESPAsync_wifiManager.resetSettings(); 
    Serial.println("reset wifi config");
    setReset(1);
  }

  //ESPAsync_wifiManager.setAPStaticIPConfig(IPAddress(192,168,132,1), IPAddress(192,168,132,1), IPAddress(255,255,255,0));
  ESPAsync_wifiManager.autoConnect(ssid);
  if (WiFi.status() == WL_CONNECTED) 
  { 
    Serial.print(F("Connected. Local IP: "));
    Serial.println(WiFi.localIP()); 
    if(getConnected()==0)
    {
      setConnected(1);
    }

    if(getOnboard()==0)
    {
      postOnboard();
    }
  }
  else
  { 
    Serial.println(ESPAsync_wifiManager.getStatus(WiFi.status())); 
  }
  //call onEvent before setup wifi mode
  //WiFi.onEvent(OnWiFiEvent);
  
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
 
  server.on("/info",HTTP_GET,[](AsyncWebServerRequest * req){
    DynamicJsonDocument json(256);
    json["status"] = st[status];
    json["ssid"] = WiFi.SSID();
    json["ip"] = WiFi.localIP().toString();
    json["uuid"]=uuid;
    String res;
    serializeJson(json, res);
    req->send(200,"application/json",res);
    });
    
  //server.on("/discard",HTTP_POST,[](AsyncWebServerRequest * req){
  server.on("/discard",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("discard");
    postDiscard();
    Serial.println("ESP restart");
    ESP.restart();
    req->send(200);
    });

  server.on("/listFile",HTTP_GET,[](AsyncWebServerRequest * req){
    storage=SD_READ;
    String res="";
    File root=SD.open("/i");
    File file=root.openNextFile();
    while(file)
    {
      delay(500);
      Serial.println(file.name());//做成HTML比較好處理
      res+="<a href=\"http://"+WiFi.localIP().toString()+String(file.name())+"\">"+String(file.name())+"</a><br>";
      file=root.openNextFile();
    }
    file.close();
    root.close();
    storage=SD_OK;
    //Serial.println(res);
    req->send(200,"text/html",res);
    });

  server.on("/upload",HTTP_GET,[](AsuncWebServerRequest * req){
    
    });
  server.begin();
  getJson();
}

void loop() 
{
  
}
