#include<DHT.h>
#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<FS.h>
#include<PubSubClient.h>
#include<NTPClient.h>
#include<WiFiUDP.h>

const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";
volatile float temp=30,humi=60;
volatile int relayStatus=0;//0:off 1:on //get value from relay side
volatile int blanketStatus=0; //0:off 1:on  
volatile float topTemp=37,lowTemp=25; //the limit of trun on relay when lower than 25 and turn off relay when over 37, it is adjestable.
String getCommandTime="None";
const char* mqttServer = "UBUNTU.local";

DHT dht11(D4,DHT11);
AsyncWebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void checkStatus()
{
  if(blanketStatus==1&&relayStatus==0)
  {
    mqttClient.publish("blanket/dht11","on");
  }

  if(blanketStatus==0&&relayStatus==1)
  {
    mqttClient.publish("blanket/dht11","off");
  }
}

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
  Serial.println(String("Temperature:")+temp);
  Serial.println(String("Humidity:")+humi);
  checkStatus();
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  String cmd="";
  for(int i=0;i<noChar;i++)
  {
    cmd+=(char)payload[i]; 
  }
  Serial.println(topic);
  //Serial.printf("payload:%u\n no boChar:%u\n",payload,noChar);
  Serial.printf("cmd:%s\n",cmd.c_str());

  if(cmd=="off")
  {
    relayStatus=0;
  }
  else if(cmd=="on")
  {
    relayStatus=1; 
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

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }

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

  server.on("/state",HTTP_GET,[](AsyncWebServerRequest * req){
    if(blanketStatus==1)
    {
      req->send(200,"text/plain",String("1"));
    }
    else
    {
      req->send(200,"text/plain",String("0"));
    }
    });
  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    if(blanketStatus==1)
    {
      mqttClient.publish("blanket/dht11","off");
      blanketStatus=0;
    }
    else if(blanketStatus==0)
    {
      mqttClient.publish("blanket/dht11","on");
      blanketStatus=1;
    }
    req->send(200);
    });
  server.on("/temperature",HTTP_GET,[](AsyncWebServerRequest *req){
    req->send(200,"text/plain",String(temp));
    });
    
  server.begin();
  Serial.println("HTTP server started"); 

  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");

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
  delay(3000);
  while(!mqttClient.connected())
  {
    mqttClient.connect("小心心的DHT11");
    mqttClient.subscribe("blanket/relay");
  }
  mqttClient.loop();
}
