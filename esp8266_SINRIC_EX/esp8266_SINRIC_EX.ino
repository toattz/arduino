/*
 * Simple example for how to use multiple SinricPro Switch device:
 * - setup 4 switch devices
 * - handle request using multiple callbacks
 * 
 * If you encounter any issues:
 * - check the readme.md at https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md
 * - ensure all dependent libraries are installed
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#arduinoide
 *   - see https://github.com/sinricpro/esp8266-esp32-sdk/blob/master/README.md#dependencies
 * - open serial monitor and check whats happening
 * - check full user documentation at https://sinricpro.github.io/esp8266-esp32-sdk
 * - visit https://github.com/sinricpro/esp8266-esp32-sdk/issues and check for existing issues or open a new one
 */

#ifdef ENABLE_DEBUG
   #define DEBUG_ESP_PORT Serial
   #define NODEBUG_WEBSOCKETS
   #define NDEBUG
#endif 

#include <Arduino.h>
#ifdef ESP8266 
   #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
   #include <WiFi.h>
#endif

#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
#include <ESP8266mDNS.h>
//#include <TridentTD_LineNotify.h>  //不可與sinric共用

#define WIFI_SSID         "ZYXEL_3F"
#define WIFI_PASS         "C1470A9A"
#define MDNS_NAME         "relay"
//#define HOSTNAME          "IOT_DEMO"
#define HOSTNAME          "啾啾王"
/*#define WIFI_SSID         "TOTOLINK"
#define WIFI_PASS         "0422876333"*/
#define APP_KEY           "4abb9bd5-5945-469b-96b0-0d2805be87fc"
#define APP_SECRET        "e9ce4bdb-beb6-490e-9513-e4ebe5eb5f71-840396ab-479d-4c38-9499-4bbef9612c9e"

//#define SWITCH_ID_1       "62bca69aad95bfbcdf32d121" //for test
#define SWITCH_ID_1       "62c840930aec232058f8009a"  //for4F
#define RELAYPIN_1        0

#define BAUD_RATE         115200                // Change baudrate to your need

AsyncWebServer server(80);

bool onPowerState1(const String &deviceId, bool &state) {
 Serial.printf("Device 1 turned %s\n", state?"on":"off");
 digitalWrite(RELAYPIN_1, state ? LOW:HIGH);  //low=>on,high=>off
 return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  WiFi.hostname(HOSTNAME);
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  pinMode(RELAYPIN_1, OUTPUT);
    
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);
  
  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
   
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }
  setupWiFi();

  if(!MDNS.begin(MDNS_NAME))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  
  server.on("/state",HTTP_GET,[](AsyncWebServerRequest * req){
    if(digitalRead(RELAYPIN_1)==HIGH)
    {
      req->send(200,"text/plain",String("0")); //off
    }
    else
    {
      req->send(200,"text/plain",String("1")); //on
    }
    });

  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    /*if(req->hasParam("val"))
    {
      AsyncWebParameter *p=req->getParam("val");
      if(p->value()=="1")
      {
        digitalWrite(relayPin,HIGH);
      }
      else
      {
        digitalWrite(relayPin,LOW);
      }
    }*/
    digitalWrite(RELAYPIN_1,!digitalRead(RELAYPIN_1));
    req->send(200);
    });

   server.on("/ip",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",WiFi.localIP().toString().c_str());
    });

   server.on("/mdns",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",MDNS_NAME+String(".local"));
    });

   server.on("/hostname",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",HOSTNAME);
    });

  server.on("/restart",HTTP_POST,[](AsyncWebServerRequest * req){
    Serial.printf("restart system\n");
    req->send(200);   
    delay(1500);
    ESP.restart(); 
    });
  server.begin();
  Serial.println("HTTP server started"); 
  
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);
  
  setupSinricPro();
}

void loop() {
  MDNS.update();
  SinricPro.handle();
}
