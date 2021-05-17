#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
const int b1=11,b2=12;//,b3=13;//buttin pins
const char* mqttServer = "192.168.1.123"; //請確認
byte mac[]={
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};//指定網路卡 MAC 位址 (可以隨便設)
EthernetClient client;// 建立乙太網物件 EthernetClient
PubSubClient mqttClient1(client);
void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  Serial.println("callback");
  char *msg=(char)payload;
  Serial.println(msg);
}
void setup() {//設定初始值
  pinMode(b1,INPUT);
  //pinMode(b2,INPUT);
  //pinMode(b3,INPUT);

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
 
  //b1Status=digitalRead(b1);
  //b2Status=digitalRead(b2);
  //Serial.println(digitalRead(b1));
  if(digitalRead(b1)==LOW)
  {
    while(digitalRead(b1)==LOW){
    }
    Serial.println("B1");
    mqttClient1.publish("w5100","B1");
  }
  /*if(digitalRead(b2)==LOW)
  {
    while(digitalRead(b2)==LOW){
    } 
    Serial.println("B2");
    //mqttClient1.publish("w5100","B2");
  }
  if(digitalRead(b3)==LOW)
   {
   while(digitalRead(b3)==LOW){} 
   Serial.println("B3");
   }*/
  while(!mqttClient1.connected()) {
    mqttClient1.connect("Arduino");
    mqttClient1.subscribe("test");
  }
  mqttClient1.loop();
} //無限迴圈
