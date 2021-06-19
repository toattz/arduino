#include<DHT.h>
#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<FS.h>
#include<PubSubClient.h>
#include<Ticker.h>
#include<NTPClient.h>
#include<WiFiUDP.h>

const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";
volatile float temp=30,humi=60;
volatile int relayStatus=0;//0:off 1:on //get value from relay side
volatile int blanketStatus=0; //0:off 1:on  
volatile float topTemp=37,lowTemp=25; //the limit of trun on relay when lower than 25 and turn off relay when over 37, it is adjestable.
String getCommandTime="None";
const char* mqttServer = "192.168.1.132";

DHT dht11(D4,DHT11);
AsyncWebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
//Ticker ticker;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void readDHT()
{
  temp=dht11.readTemperature();
  humi=dht11.readHumidity();
  if(temp<lowTemp)
  {
    blanketStatus=1;
  }

  if(temp>topTemp)
  {
    blanketStatus=0;
  }
  /*Serial.print("Temperature:");
  Serial.println(temp);
  Serial.print("Humidity:");    
  Serial.println(humi);*/
}

void checkStatus()
{
  if(blanketStatus==1&&relayStatus==0)
  {
    mqttClient.publish("blanket/dht11","1");
  }

  if(blanketStatus==0&&relayStatus==1)
  {
    mqttClient.publish("blanket/dht11","0");
  }
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  char v = (char) payload[0];
  Serial.printf("topic:%s\n",topic);
  Serial.print("value:");
  Serial.println(v);
  timeClient.update();
  getCommandTime=timeClient.getFormattedTime();
  Serial.println(getCommandTime);

  if(v=='1')
  {
    relayStatus=1;
  }
  else if(v=='0')
  {
    relayStatus=0; 
  }
  else
  {
    //Serial.println("invalid command");
  }
}

void setup() 
{
  Serial.begin(115200);
  dht11.begin();

  WiFi.mode(WIFI_STA);
  WiFi.hostname("小心心的DHT11");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nWiFi connected\nIP address:%s\n",WiFi.localIP().toString().c_str());
  
  //tickDHT.attach(5,readDHT);
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");

  server.begin();
  Serial.println("HTTP server started"); 

  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");

  //ticker.attach(5,sendStatus);

  timeClient.begin();
  // 設定時區 *60分 * 60秒，例如:
  // GMT +1 = 3600
  // GMT +8 = 28800  台灣時區
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
  
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
}

void loop() {
  readDHT();
  delay(30000);
  while(!mqttClient.connected())
  {
    mqttClient.connect("小心心的DHT11");
    mqttClient.subscribe("blanket/relay");
  }
  mqttClient.loop();
}
