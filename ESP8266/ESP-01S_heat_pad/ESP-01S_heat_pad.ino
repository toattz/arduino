#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<FS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define RELAYPIN 0

int ssid_sr=1;
const char * ssid[]={"TOTOLINK",
                     "TP-LINK_470A9A",
                     "ZYXEL_1F",
                     "ZYXEL_3F"};
const char * password[]={"0422876333",
                         "C1470A9A",
                         "C1470A9A",
                         "C1470A9A"};
volatile bool heat[]={false,false,false,false,false,false,
                      false,false,false,false,false,false,
                      false,false,false,false,false,false,
                      false,false,false,false,false,false};
volatile bool manual=false;
volatile byte id=0,val=0;

AsyncWebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"ntp1.aliyun.com",60*60*8,30*60*1000);

void setup()
{
  Serial.begin(115200);
  Serial.println("start system");

  pinMode(RELAYPIN,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.hostname("心心爬蟲加熱墊");
  WiFi.begin(ssid[ssid_sr],password[ssid_sr]);
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
    String res="{\"status\":";
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

  //choose manual heat or automtic heat
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

  //automatic heat or not when manual is false
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

  //set heart timezone 
  server.on("/set",HTTP_GET,[](AsyncWebServerRequest * req)
  {
    Serial.println("set"); 
    if(req->hasParam("id"))
    {
      AsyncWebParameter * p=req->getParam("id");
      id=p->value().toInt();
      Serial.println(id);  
    }

    if(req->hasParam("val"))
    {
      AsyncWebParameter * p=req->getParam("val");
      val=p->value().toInt();
      Serial.println(val);  
    }    
    if(val==true)
    {
      heat[id]=true;
    }
    else
    {
      heat[id]=false;
    }
    req->send(200);
    });

  //get heat timezone one by one
  server.on("/heat",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("heat");
    if(req->hasParam("id"))
    {
      AsyncWebParameter * p=req->getParam("id");
      id=p->value().toInt();
      Serial.println(id);  
    }
    if(heat[id]==true)
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

  //get heat timezone at a time
  server.on("/heat2",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.println("heat2");
    for(byte i=0;i<24;i++)
    {
      if(heat[i]==true)
      {
        
      }
      else
      {
        
      }
    }
    /*if(req->hasParam("id"))
    {
      AsyncWebParameter * p=req->getParam("id");
      id=p->value().toInt();
      Serial.println(id);  
    }
    if(heat[id]==true)
    {
      Serial.println("true");
      req->send(200,"text/plain",String(1));
    }
    else
    {
      Serial.println("false");
      req->send(200,"text/plain",String(0));
    }*/
    req->send(200,"text/plain",String(0));
    });

  
  server.begin();
  Serial.println("Web server is started");
  timeClient.begin();
}

void loop()
{
  //digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());

  if(manual==false)
  {
    if(heat[timeClient.getHours()]==true)
    {
      Serial.println("heat");
      
    }
    else
    {
      Serial.println("not heat");
    }
  }
  /*for(byte i=0;i<24;i++)
  {
    Serial.printf("%d~%d:%d\n",i,i+1,heat[i]);
  }*/
  delay(3000);
}
