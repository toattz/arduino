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

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <TridentTD_LineNotify.h>
#include <Ticker.h>
#include <DHT.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define WIFI_SSID         "TOTOLINK"
#define WIFI_PASS         "0422876333"
/*#define WIFI_SSID         "ZYXEL_1F"
#define WIFI_PASS         "C1470A9A"*/
#define MDNS_NAME         "line"
#define HOSTNAME          "LINE_NOTIFY_DEMO"
#define LINE_TOKEN        "V5xuingKhVKe8Hd5jRmwIypMPwxqFyp3trLmCIRiVbd"
#define BAUD_RATE         115200                // Change baudrate to your need
#define RELAY             D1
#define DHTTYPE           DHT11
#define DHTPIN            D4

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
Ticker tickMemory;
Ticker tickDHT;
bool sendLine=false;
bool sendDHT=false;
int count=0;
String msg="";

void memoryNotify()
{
  msg="";
  long  fh = ESP.getFreeHeap();
  char  fhc[20];

  ltoa(fh, fhc, 10);
  msg="Memory:"+String(fhc);
  sendLine=true;
}

void DHTNotify()
{
  sendDHT=true;
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
    req->send(200,"text/plain",String("1"));
    });

  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    Serial.printf("開關繼電器\n");
    msg="開關繼電器";
    sendLine=true;
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

   server.on("/line",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.printf("測試line\n");
    msg="測試line";
    sendLine=true;
    req->send(200);
    });

   server.on("/dht",HTTP_GET,[](AsyncWebServerRequest * req){
    Serial.printf("測試DHT\n");
    sendDHT=true;
    req->send(200);
    });

  server.on("/restart",HTTP_POST,[](AsyncWebServerRequest * req){
    Serial.printf("restart system\n");
    msg="restart system";
    sendLine=true;
    req->send(200);   
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
  
  //setupSinricPro();

  LINE.setToken(LINE_TOKEN);
  LINE.notify("\nsetup");
  tickMemory.attach(1800,memoryNotify);
  tickDHT.attach(1200,DHTNotify);
  dht.begin();
}

void loop() {
  // LINE.notify放在 server.on 內會失敗  所以放在loop
  if(sendLine==true)
  {
    Serial.println("sendLine");
    int ret=LINE.notify(msg);
    Serial.printf("ret:%d\n",ret);
    sendLine=false;
  }

  if(sendDHT==true)
  {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.printf("sendDHT temperater:%s  humidity:%s",String(t,2),String(h,2));
    //String url="http://ubuntu.local:8000/getDHT11/"+String(t,2)+"/"+String(h,2)+"/";
    String url="http://toattz.pythonanywhere.com/getDHT11/"+String(t,2)+"/"+String(h,2)+"/";
    Serial.println(url);
    LINE.notify(url);
    WiFiClient client;
    HTTPClient httpClient;
    httpClient.begin(client,url); 
    httpClient.GET();
    httpClient.end();
    sendDHT=false;
  }
  MDNS.update();
}
