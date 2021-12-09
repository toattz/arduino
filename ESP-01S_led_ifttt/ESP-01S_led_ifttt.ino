#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<aREST.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
const char * device_id="baxyz";

WiFiClient espClient;
PubSubClient client(espClient);
aREST rest=aREST(client);

void toggle()
{
  digitalWrite(LED_BUILTIN,digitalRead(LED_BUILTIN));
}

void callback(char *topic,byte *payload, unsigned int length)
{
  rest.handle_callback(client,topic,payload,length);
}

void setup() 
{
  Serial.begin(115200);

  client.setCallback(callback);
  
  rest.set_id(device_id);
  rest.set_name("led");

  pinMode(LED_BUILTIN,OUTPUT);
  
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
}

void loop() 
{
  rest.handle(client);

}
