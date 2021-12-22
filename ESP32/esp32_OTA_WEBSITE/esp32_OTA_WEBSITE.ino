#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<Update.h>
#include<SPIFFS.h>
#include<ESPmDNS.h>

const char * ssid="TOTOLINK N150RT";
const char * password="0422876333";
const char * host="blanka";

AsyncWebServer server(80);

void handleUpdate(AsyncWebServerRequest *req,String filename,size_t index,uint8_t *data,size_t len,bool final)
{
  if(!index)
  {
    Serial.printf("Update firmware:%s\n",filename.c_str());
    if(!Update.begin())
    {
      Update.printError(Serial);
    }
  }

  if(len)
  {
    Update.write(data,len);
  }

  if(final)
  {
    if(Update.end(true))
    {
      Serial.printf("Wrote %u bytes\nRestart ESP32",index+len);
    }
    else
    {
      Update.printError(Serial);
    }
  }
}

void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.hostname("OTA_WEBSITE");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
  
  if(!MDNS.begin(host))
  {
    Serial.println("Setup MDNS fail");
    while(1)
    {
      delay(50);
    }
  }

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/firmware",SPIFFS,"/www/firmware.html");
  server.on("/upload",HTTP_POST,[](AsyncWebServerRequest *req){
    req->send(200,"text/html;charset=utf-8",(Update.hasError()?"Update fail":"Update success"));
    delay(3000);
    ESP.restart();
    },handleUpdate);
  server.begin();
  MDNS.setInstanceName("CCC ESP32");
  MDNS.addService("http","tcp",80);
  

}

void loop()
{
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);

}
