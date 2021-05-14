#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#define dhtPin D1   //DHT11 DATA 数据引脚
#define DHTTYPE DHT11  //选择的类型
#define relayPin D2 //for relay
#define firePin D3  //
#define vibrationPin A0  //for GY-61

char* id = "1";// device id
const char* ssid = "TP-LINK_470A9A";//請查無線網路名稱
const char* password = "C1470A9A"; //請查密碼
const char* mqttServer = "10.1.1.44"; //請確認
WiFiClient wifiClient1;
PubSubClient mqttClient1(wifiClient1);
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
char temptostr[10]; 
char humitostr[10]; 
DHT dht(dhtPin, DHTTYPE);
String formattedDate;
String dayStamp;
String timeStamp;
char state[100] = "";
int fireStatus = 0;
int vibrationX = 0;

// 將類比輸入的值轉成g
float toG(float v){
  return v * 6 / 1023 - 3;
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar) {
  /*char *response;
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
  mqttClient1.publish("ack", response);*/
}
void setup() { 
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

  dht.begin();  

  pinMode(relayPin, OUTPUT);
  pinMode(firePin ,INPUT_PULLUP);
  pinMode(vibrationPin, INPUT);
  digitalWrite(relayPin, HIGH);
  
}
void loop() {
  //get DHT-11 value
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  itoa(t,temptostr, 10); 
  itoa(h,humitostr, 10);   
  delay(500);

  //read Vibration Pin Value
  vibrationX  = analogRead(vibrationPin);

  //read Fire Pin value
  if(digitalRead(firePin) == 0)
  {
    fireStatus = 1;
  }
  else
  {
    fireStatus = 0;
  }
  while( !mqttClient1.connected()) {
    mqttClient1.connect("ESP32");
    mqttClient1.subscribe("command");
  }
  mqttClient1.loop();
  //send data to json type for apache
  char state[100] = "";
  strcat(state, "{\"id\":");
  strcat(state, id);
  strcat(state, ",\"temp\":");
  strcat(state, temptostr);
  strcat(state, ",\"humi\":");
  strcat(state, humitostr);
  strcat(state, "}");
  Serial.print(state);
  Serial.print("\n");
  Serial.print("Vibration : ");
  Serial.print(toG(vibrationX));
  Serial.print("\n");
  Serial.print("Fire : ");
  Serial.print(fireStatus);
  Serial.print("\n");
  
  mqttClient1.publish("ack", state);
  
  delay(5000);
}
