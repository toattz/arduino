#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<FS.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

//ESP-01S
#define relayPin 15// relay connected to  GPIO0

const char* ssid="ZYXEL_1F";
const char* password="C1470A9A";
const char* mqttServer = "192.168.1.132";
String getCommandTime="None";

// Set web server port number to 80
AsyncWebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Ticker ticker;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
//WebSocketsServer webSocket=WebSocketsServer(81);

/*void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  if(type==WStype_TEXT)
  {
    //Serial.printf("[%u] get Text: %s\n%zu\n", num, payload,length);
    char s=(char)payload[0];
    int state=atoi(&s);
    if(state==1)
    {
      digitalWrite(relayPin,HIGH);
    }
    else
    {
      digitalWrite(relayPin,LOW);
    }
  }
}*/

void sendStatus()
{
  if(digitalRead(LED_BUILTIN)==HIGH)
  {
    mqttClient.publish("blanket/relay","off");
  }
  else
  {
    mqttClient.publish("blanket/relay","on");
  }
}

/*String getCurrentDate()
{
  String currentTime="";
  String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();
  //Get a time structure
  struct tm *ptm=gmtime ((time_t *)&epochTime); 
  currentTime+=String(ptm->tm_year+1900);
  currentTime+=" ";
  currentTime+=months[ptm->tm_mon];
  currentTime+=" ";
  currentTime+=String(ptm->tm_mday);
  currentTime+=" ";
  currentTime+=timeClient.getFormattedTime();
  return currentTime;
}*/

void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  char v = (char) payload[0];
  Serial.print("value:");
  Serial.println(v);
  timeClient.update();
  getCommandTime=timeClient.getFormattedTime();
  Serial.println(getCommandTime);

  if(v=='1')
  {
    digitalWrite(LED_BUILTIN,HIGH);
    //mqttClient.publish("blanket/relay","off");
  }
  else if(v=='0')
  {
    digitalWrite(LED_BUILTIN,LOW);
    //mqttClient.publish("blanket/relay","on");
  }
  else
  {
    //Serial.println("invalid command");
  }
}

void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
 
  Serial.begin(115200);

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }
  // Connect to Wi-Fi network with SSID and password
  WiFi.mode(WIFI_STA);
  //WiFi.hostname("ESP-01S_CAT_FAN_RELAY_V2");
  WiFi.hostname("小心心的繼電器");
  Serial.printf("Connecting to %s\n",ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.printf("IP address: %s\n",WiFi.localIP().toString().c_str());
  
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  
  server.on("/state",HTTP_GET,[](AsyncWebServerRequest * req){
    if(digitalRead(relayPin)==HIGH)
    {
      req->send(200,"text/plain",String("0"));
    }
    else
    {
      req->send(200,"text/plain",String("1"));
    }
    });

  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    digitalWrite(relayPin,!digitalRead(relayPin));
    req->send(200);
    });
  server.on("/time",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",String(getCommandTime));
    });
  server.begin();
  Serial.println("HTTP server started"); 
  
  /*webSocket.onEvent(webSocketEvent);
  webSocket.begin(); 
  Serial.println("Websock is started");*/
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");

  ticker.attach(5,sendStatus);

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

void loop()
{
  //webSocket.loop();
  while(!mqttClient.connected())
  {
    mqttClient.connect("小心心的繼電器");
    mqttClient.subscribe("blanket/dht11");
  }
  mqttClient.loop();
}
