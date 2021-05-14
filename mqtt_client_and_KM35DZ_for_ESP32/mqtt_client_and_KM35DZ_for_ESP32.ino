#include <WiFi.h> //for ESP32
//#include <ESP8266WiFi.h> //for ESP8266
#include <PubSubClient.h>
int tempPin=15;
int readValue = 0;
int temp = 0;
int degreeF;
int voltage=330; //330for 3.3V, 500 for 5V, 110 for 1.1V
char temptostr[10]; 
const char* ssid = "TP-LINK_470A9A";//請查無線網路名稱
const char* password = "C1470A9A"; //請查密碼
const char* mqttServer = "10.1.1.25"; //請確認
WiFiClient wifiClient1;
PubSubClient mqttClient1(wifiClient1);
void receivedCMD(char* topic, byte* payload, unsigned int noChar) {
  char *response;
  char cmd = (char) payload[0];
  Serial.println(cmd);
  if ( cmd == '1') { //return Celsius °C
    readValue = analogRead(tempPin);
    //temp = map(readValue, 0, 1023, 0, 330); //for ESP8622
    temp = map(readValue, 0, 4095, 0, 330)*0.1;//for ESP32
    Serial.print("The degree Celsius = ");
    Serial.println(temp);  
    itoa(temp,temptostr, 10); 
    //Strcat(response, "The degree Celsius = ", temptostr);
    //response = "ok 1";
    response=temptostr;
  } 
  else if ( cmd == '2')  {// return Fahrenheit ℉
    readValue = analogRead(tempPin);
    //temp = map(readValue, 0, 1023, 0, 330); //for ESP8622
    temp = map(readValue, 0, 4095, 0, 330)*0.1;//for ESP32
    Serial.print("The degree Fahrenheit = ");
    degreeF = ((float) temp)*9/5 + 32.0;
    Serial.println(degreeF); 
    itoa(degreeF,temptostr, 10); 
    //Strcat(response, "The degree Fahrenheit = ", temptostr);
    //response = "ok 2";
    response=temptostr;
  }
  else if ( cmd == '3')  {
    response = "Got 3";
  }
  else if ( cmd == '0') {
    response = "Got 0";
  }
  else {
    response = "NO function is supported";
  }
  mqttClient1.publish("ack", response);
}
void setup() 
{ 
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
