#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
const int b1=5,b2=9,b3=8;//buttin pins
const int gLed=6,yLed=7;
const int maxDebounce=500;
unsigned int debounce=0;
const char* mqttServer = "192.168.1.123"; //請確認
byte mac[]={
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};//指定網路卡 MAC 位址 (可以隨便設)
EthernetClient client;// 建立乙太網物件 EthernetClient
PubSubClient mqttClient1(client);
void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  Serial.println("callback");
  char msg=(char)payload[0];
  //Serial.println((char)payload[0]);
  Serial.println(msg);
  digitalWrite(yLed,HIGH);
  delay(2000);
  digitalWrite(yLed,LOW);
  
}
void setup() {//設定初始值
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  pinMode(b3,INPUT_PULLUP);
  pinMode(gLed, OUTPUT);
  pinMode(yLed, OUTPUT);
  digitalWrite(gLed,LOW);
  digitalWrite(yLed,LOW);

  Serial.begin(115200);//初始化序列埠
  //啟用 Ethernet 連線，預設會以 DHCP 取得 IP 位址
  if (Ethernet.begin(mac)==0) {
    Serial.println("Cannot get IP Address!");   
    for(;;);// 無法取得 IP 位址，不做任何事情
  }
  Serial.print("IP Address ");//輸出 IP 位址
  Serial.println(Ethernet.localIP());
  mqttClient1.setServer(mqttServer, 1883);
  mqttClient1.setCallback(receivedCMD);
}
void loop() 
{
  if(digitalRead(b1)==LOW)
  { 
    if(debounce>maxDebounce)
    {   
      digitalWrite(gLed,HIGH);
      while(digitalRead(b1)==LOW){
  
      }
      debounce=0;
      digitalWrite(gLed,LOW);
      Serial.println("B1");
      mqttClient1.publish("w5100","B1");
    }
    else
      debounce++;
  }
  if(digitalRead(b2)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(b2)==LOW)
      {
        digitalWrite(gLed,HIGH);
        delay(500);
        digitalWrite(gLed,LOW);
        delay(500);
      } 
      debounce=0;
      digitalWrite(gLed,LOW);
      Serial.println("B2");
      mqttClient1.publish("w5100","B2");
    }
    else
      debounce++;
  }
  if(digitalRead(b3)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(b3)==LOW)
      {
        digitalWrite(gLed,HIGH);
        delay(200);
        digitalWrite(gLed,LOW);
        delay(200);
      }
      debounce=0;
      digitalWrite(gLed,LOW);
      Serial.println("B3");
      mqttClient1.publish("w5100","B3");
    }
    else
      debounce++;
  }
  /*digitalWrite(gLed,LOW);
  delay(2000);
  digitalWrite(gLed,HIGH);
  delay(2000);*/
  while(!mqttClient1.connected()) {
    mqttClient1.connect("Arduino");
    mqttClient1.subscribe("test");
  }
  mqttClient1.loop();
} //無限迴圈
