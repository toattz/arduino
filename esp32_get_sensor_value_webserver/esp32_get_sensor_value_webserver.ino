#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<SPIFFS.h>

//#define PIN 15
int i=0;
const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";

AsyncWebServer server(80);

void setup() {
  //pinMode(PIN,INPUT);
  // put your setup code here, to run once:
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    return;
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("Get_sensor_value");
  WiFi.begin(ssid,password);

  while (WiFi.status()!=WL_CONNECTED)
    delay(500);

  Serial.printf("IP address:%s\n",WiFi.localIP().toString().c_str());
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon",SPIFFS,"/www/favicon.ico");
  server.on("/LDR",HTTP_GET,[](AsyncWebServerRequest * req){
    uint16_t i=analogRead(32);
    req->send(200,"text/plain",String(i));
    });
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //i=analogRead(32);
  //Serial.println(i);

}
