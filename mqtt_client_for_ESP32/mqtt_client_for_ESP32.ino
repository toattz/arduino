#include <WiFi.h>
#include <PubSubClient.h>
int LEDPin1=4;
int LEDPin2=2;
int LEDPin3=15;

const char* ssid = "TP-LINK_470A9A";//請查無線網路名稱
const char* password = "C1470A9A"; //請查密碼
const char* mqttServer = "10.1.1.25"; //請確認
WiFiClient wifiClient1;
PubSubClient mqttClient1(wifiClient1);
void receivedCMD(char* topic, byte* payload, unsigned int noChar) {
  char *response;
  char LED = (char) payload[0];
  Serial.println(LED);
  if ( LED == '1') {
    digitalWrite(LEDPin1, HIGH);
    digitalWrite(LEDPin2, LOW);  
    digitalWrite(LEDPin3, LOW);   
    response = "ok 1";
  } 
  else if ( LED == '2')  {
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);  
    digitalWrite(LEDPin3, LOW); 
    response = "ok 2";
  }
  else if ( LED == '3')  {
    digitalWrite(LEDPin1, LOW); 
    digitalWrite(LEDPin2, LOW);
    digitalWrite(LEDPin3, HIGH);  
    response = "ok 3";
  }
  else if ( LED == '0') {
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, LOW);
    digitalWrite(LEDPin3, LOW); 
    response = "ok 0";
  }
  else {
    response = "Not ok";
  }
  mqttClient1.publish("ack", response);
}
void setup() {
  pinMode(LEDPin1, OUTPUT);    
  pinMode(LEDPin2, OUTPUT);
  pinMode(LEDPin3, OUTPUT);
  digitalWrite(LEDPin1,LOW);
  digitalWrite(LEDPin2,LOW);
  digitalWrite(LEDPin3,LOW);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) //等待网络连接成功
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //打印模块IP
  mqttClient1.setServer(mqttServer, 1883);
  mqttClient1.setCallback(receivedCMD);
}
void loop() {
  while( !mqttClient1.connected()) {
    mqttClient1.connect("ESP32");
    mqttClient1.subscribe("command");
   }
  mqttClient1.loop();
}
