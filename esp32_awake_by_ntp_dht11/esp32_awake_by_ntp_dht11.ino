#include<WiFi.h>
#include"sleepTimer.h"
#include<DHT.h>
#include<HTTPClient.h>
#define MIN 30
#define SEC 0
#define DHTTYPE DHT11
#define DHTPIN 15

DHT dht(DHTPIN,DHTTYPE);

const char * ssid="TOTOLINK N150RT";
const char * password="0422876333";
String apikey="P87EVA49G8HV2WWB";
SleepTimer st(100);

void sendData()
{
  float t=dht.readTemperature();
  float h=dht.readHumidity();
  HTTPClient http;
  String url="https://api.thingspeak.com/update?api_key=0"+apikey;
  url+="&field1=";
  url+=String(t);
  url+="&field2=";
  url+=String(h);
  url+=" HTTP/1.1\n";
  http.begin(url);
  int httpCode=http.GET();
  //Serial.println("Send data");

  if(httpCode>0)
  {
    String payload=http.getString();
    Serial.printf("Http code:%\d, response:%s\n",httpCode,payload); 
  }
  else
  {
    Serial.println("http request is wrong");
  }
  http.end();
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("wake up bt ntp");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  dht.begin();
  st.init(28800,0);
  int res=st.start(MIN,SEC,sendData);
  Serial.printf("res:%d\n",res);

}

void loop() {
  // put your main code here, to run repeatedly:

}
