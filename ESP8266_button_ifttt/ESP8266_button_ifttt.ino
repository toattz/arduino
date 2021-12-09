#include<ESP8266WiFi.h>

const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";
const char *host="maker.ifttt.com";
const char *eventName="button_pressed";
const char *key="key";

unsigned int debounce=0;
byte maxDebounce=500;

void setup() 
{
  Serial.begin(115200);

  pinMode(5,INPUT_PULLUP);
  
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
  if(digitalRead(5)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(5)==LOW)
      {
        
      }
      debounce=0;
      Serial.println("bb");

      WiFiClient client;
      const int httpPort=80;
      if(!client.connect(host,httpPort))
      {
        Serial.println("connection failed");
        return;
      }

      String url="/trigger/";
      url+=eventName;
      url+="/with/key/";
      url+=key;
      Serial.printf("Requesting URL: %s\n",url.c_str());

      client.print(String("GET ")+url+" HTTP/1.1\r\n"+
                   "Host: "+host+"\r\n"+
                   "Connection close\r\n\r\n");
      int timeout=millis()+5000;
      while(client.available()==0)
      {
        if(timeout-millis()<0)
        {
          Serial.println(">>> Client Timeout");
          client.stop();
          return;
        }
      }

      while(client.available())
      {
        String line=client.readStringUntil('\r');
        Serial.print(line);
      }
      Serial.printf("close connection\n");


      
    }
    else
    {
      debounce++;
    }
    
  }

}
