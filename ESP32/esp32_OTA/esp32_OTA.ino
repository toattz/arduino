#include<WiFi.h>
#include<ArduinoOTA.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("OTA_DEMO");
  WiFi.begin(ssid,password);

  while(WiFi.waitForConnectResult()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
  
  ArduinoOTA.setHostname("ESP32 IOT");
  ArduinoOTA.setPassword("12345");
  ArduinoOTA.begin();

}

void loop()
{
  ArduinoOTA.handle();
  digitalWrite(LED_BUILTIN,LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
}
