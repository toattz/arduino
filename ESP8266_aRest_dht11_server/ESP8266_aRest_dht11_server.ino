#include<ESP8266WiFi.h>
#include<aREST.h>
#include<DHT.h>
//IP/temperature
//ip/humidity
#define DHT_TYPE DHT11
#define DHT_PIN 5
#define PORT 80

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
float temperature;
float humidity;

DHT dht(DHT_PIN,DHT_TYPE,15);
WiFiServer server(PORT);
aREST rest=aREST();

void setup()
{
  Serial.begin(115200);
  dht.begin();

  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);
  rest.set_id("1");
  rest.set_name("esp8266");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
  server.begin();
}

void loop()
{
  //Serial.printf("H:%f T:%f\n",dht.readHumidity(),dht.readTemperature());
  temperature=dht.readTemperature();
  humidity=dht.readHumidity();
  
  WiFiClient client=server.available();
  if(!client)
    return;

  while(!client.available())
  {
    delay(1);
  }

  rest.handle(client);
}
