#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPmDNS.h>

const char *ssid = "ZYXEL_1F";
//const char *ssid = "TP-LINK_470A9A";
const char *password = "C1470A9A";

AsyncWebServer server(80);

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("mDNS");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  if(!MDNS.begin("blanka"))
  {
    while(1)
    {
      
    }
  }

  server.on("/",HTTP_GET,[](AsyncWebServerRequest *req){
    req->send(200,"text/html;charset=utf-8","ESP32");
    });
  server.begin();

  MDNS.setInstanceName("I have esp32");
  MDNS.addService("http","tcp",80);
}

void loop() {
  // put your main code here, to run repeatedly:

}
