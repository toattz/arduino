#include<ESP8266WiFi.h>
#include<PubSubClient.h>
#include<Ticker.h>

const char* ssid="ZYXEL_1F";
const char* password="C1470A9A";
const char* mqttServer = "192.168.1.132";
volatile uint32_t i=0;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Ticker ticker1,ticker2;

void sendData()
{
  mqttClient.publish("command",String(i).c_str());
  i++;
}

void ledBlink()
{
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  String cmd="";
  for(int i=0;i<noChar;i++)
  {
    cmd+=(char)payload[i]; 
  }
  Serial.printf("cmd:%s\n",cmd.c_str());
}

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
 
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("TX_DATA");
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
  
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");
  ticker1.attach(60,sendData);
  ticker2.attach(1,ledBlink);
}

void loop()
{
  while(!mqttClient.connected())
  {
    mqttClient.connect("ESP-01S");
    mqttClient.subscribe("nothing");
  }
  mqttClient.loop();
}
