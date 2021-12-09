#include<ESP8266WiFi.h>
//#include<aREST.h>
#include<PubSubClient.h>
#include <aREST.h>

WiFiClient espClient;
PubSubClient client(espClient);
aREST rest=aREST(client);

const char * dev_id="blanka6711";
const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";

void callback(char *topic,byte *payload,unsigned int length)
{
  rest.handle_callback(client,topic,payload,length);  
}
void setup() 
{
  Serial.begin(115200);
  
  client.setCallback(callback);

  rest.set_id(dev_id);
  rest.set_name("devices_control");

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
  Serial.println(LED_BUILTIN);

  const char *out_topic=rest.get_topic().c_str();

}

void loop() 
{
  rest.handle(client);

}
