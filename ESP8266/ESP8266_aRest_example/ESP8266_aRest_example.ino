#include<ESP8266WiFi.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
const char * host="www.example.com";


void setup()
{
  Serial.begin(115200);
    
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
  Serial.printf("Connect to %s\n",host);

  WiFiClient client;
  const int httpPort=80;
  if(!client.connect(host,httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  //client.setTimeout(2000);
  client.print(String("GET /")+" HTTP/1.1\r\nHost: "+host+"\r\nConnection: close\r\n\r\n");
  delay(10);
  //byte timeout=millis()+5000;
  while(client.available()==0)
  {
    /*if(timeout-millis()<0)
    {
      Serial.println("==============================================================");
      Serial.println("Stop client");
      Serial.println("==============================================================");
      client.stop();
      //return;
    }
    String line=client.readString();*/
    String line=client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.printf("\nConnection close\n");
  delay(5000);
}
