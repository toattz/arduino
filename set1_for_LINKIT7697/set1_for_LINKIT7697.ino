#include <LiquidCrystal_I2C.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <LWiFi.h>
#define dhtPin 4   //DHT11 DATA 数据引脚
#define DHTTYPE DHT11  //选择的类型
#define relayPin 3 //for relay
#define firePin 2  //

char *setN[] = {"set1", "set2", "set3"};
int counter = 0;
int id = 2;// device id
int lcdStatus = 0; //0:normal, 1:maintenance 2:Over Temperature, 3: Over Humidity, 4:On fire, 5:Over vibration 6:Offline
int lcdPolling = 0; //0:IP address, 1:Temperature, 2:Humidity, 3:No fire, 4:Vibration
const char* ssid = "TP-LINK_470A9A";//請查無線網路名稱
const char* password = "C1470A9A"; //請查密碼
const char* mqttServer = "10.1.1.102"; //請確認
LiquidCrystal_I2C lcd_i2c(0x27);
WiFiClient client;
PubSubClient mqttClient1(client);
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
DHT dht(dhtPin, DHTTYPE);
int fireStatus = 0;
//int xValue = 0;
int yValue = 0; //pin14
//int zValue = 0;
float h, t;


void sendJson(int temp, int humi, int fire, int vibration)
{ 
  char str[200];
  sprintf(str,"");
  sprintf(str, "{\"id\":%d,\"temp\":%d,\"humi\":%d,\"fire\":%d,\"vibration\":%d}", id, temp, humi, fire, vibration);
  mqttClient1.publish("ack", str);  
}

void writeDB(int temp, int humi, int fire, int vibration)
{

  char str[200];
  sprintf(str,"");
  sprintf(str, "GET /%s.php?temp=%d&humi=%d&fire=%d&vibration=%d HTTP/1.0", setN[id -1], temp, humi, fire, vibration);

  if(client.connect(mqttServer, 80))
  {
    Serial.println("connected to server (GET)");
    // Make a HTTP request:
    //client.println("GET /set1.php?temp=100&humi=100&fire=10&vibration=1000 HTTP/1.0");
    client.println(str);
    sprintf(str,"");
    sprintf(str,"Host: %s", mqttServer);
    //client.println("Host: 10.1.1.102");
    client.println(str);
    client.println("Accept: */*");
    client.println("Connection: close");
    client.println();
    delay(10);
  }
  client.stop();
}

void lcdMessage()
{
  if(lcdStatus == 0)
  {
    if(lcdPolling == 0)
    {
      lcd_i2c.clear();
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("IP Address");
      lcd_i2c.setCursor(0,1);
      lcd_i2c.print(WiFi.localIP());
      lcdPolling = lcdPolling + 1;
    }
    else if(lcdPolling == 1)
    {
      lcd_i2c.clear();      
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("Temperature:");
      lcd_i2c.setCursor(12,0);
      lcd_i2c.print(int(t));
      lcd_i2c.setCursor(14,0);
      lcd_i2c.print((char) 0xDF);
      lcd_i2c.setCursor(15,0);
      lcd_i2c.print("C");
      lcd_i2c.setCursor(0,1);
      lcd_i2c.print("Running"); 
      lcdPolling = lcdPolling + 1;     
    }
    else if(lcdPolling == 2)
    {
      lcd_i2c.clear();      
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("Humidity:");
      lcd_i2c.setCursor(9,0);
      lcd_i2c.print(int(h)); 
      lcd_i2c.setCursor(11,0);
      lcd_i2c.print("%"); 
      lcd_i2c.setCursor(0,1);
      lcd_i2c.print("Running");        
      lcdPolling = lcdPolling + 1;
    }
    else if(lcdPolling == 3)
    {
      lcd_i2c.clear();      
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("No fire");
      lcd_i2c.setCursor(0,1);
      lcd_i2c.print("Running");        
      lcdPolling = lcdPolling + 1;
    }
    else
    {
      lcd_i2c.clear();      
      lcd_i2c.setCursor(0,0);
      lcd_i2c.print("Vibration:");
      lcd_i2c.setCursor(10,0);
      lcd_i2c.print(yValue);
      lcd_i2c.setCursor(0,1);
      lcd_i2c.print("Running"); 
      lcdPolling = 0;     
    }
  }
  else if(lcdStatus == 1)
  {
    lcd_i2c.clear();    
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("Maintenace mode");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print("Stop machine");     
  }
  else if(lcdStatus == 2)
  {
    lcd_i2c.clear();    
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("Over Temperature");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print("Stop machine");     
  }
  else if(lcdStatus == 3)
  {
    lcd_i2c.clear();    
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("Over Humidity");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print("Stop machine");     
  }
  else if(lcdStatus == 4)
  {
    lcd_i2c.clear();    
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("On fire");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print("Stop machine");     
  }
  else
  {
    lcd_i2c.clear();    
    lcd_i2c.setCursor(0,0);
    lcd_i2c.print("Over vibration");
    lcd_i2c.setCursor(0,1);
    lcd_i2c.print("Stop machine");     
  }
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar) {
  //char *response;
  char LED = (char) payload[0];
  Serial.print("LED:");
  Serial.println(LED);
  if ( LED == '0')
  {
    digitalWrite(relayPin, LOW);
    lcdStatus = 0;
  } 
  else if ( LED == '1')
  {
    digitalWrite(relayPin, HIGH);
    lcdStatus = 1;
  }
  else if ( LED == '2')  {
    digitalWrite(relayPin, HIGH);
    lcdStatus = 2;
  }
  else if ( LED == '3') {
    digitalWrite(relayPin, HIGH);
    lcdStatus = 3;
  }
  else if ( LED == '4') {
    digitalWrite(relayPin, HIGH);
    lcdStatus = 4;
  } 
  else if ( LED == '5') {
    digitalWrite(relayPin, HIGH);
    lcdStatus = 5;
  }
  else
  {
    //response = "Not ok";
  }
  lcdMessage();
  //mqttClient1.publish("ack", response);*/
}
void setup() { 
  Serial.begin(115200);
  lcd_i2c.begin(16, 2);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) //等待网络连接成功
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //打印模块IP
  lcd_i2c.setCursor(0,0);
  lcd_i2c.print(WiFi.localIP());
  mqttClient1.setServer(mqttServer, 1883);
  mqttClient1.setCallback(receivedCMD);
  lcd_i2c.setCursor(0,1);
  lcd_i2c.print("MQTT CONNECTED");
  dht.begin();  
  
  pinMode(relayPin, OUTPUT);
  pinMode(firePin ,INPUT_PULLUP);
  digitalWrite(relayPin, LOW);
  
}
void loop() {
  //get DHT-11 value
  h = dht.readHumidity();
  t = dht.readTemperature();
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
  delay(500);

  //read Vibration Pin Value

  //read Fire Pin value
  if(digitalRead(firePin) == 0)
  {
    fireStatus = 1;  //on fire
  }
  else
  {
    fireStatus = 0;  // no filre
  }

  yValue = analogRead(14); //get ibration value
  while( !mqttClient1.connected()) {
    mqttClient1.connect(setN[id - 1]);
    mqttClient1.subscribe(setN[id - 1]);
  }
  mqttClient1.loop();
  
  Serial.println(h);
  Serial.println(t);
  Serial.print("Fire : ");
  Serial.println(fireStatus);
  Serial.print("vibration: ");
  Serial.println(yValue);
  
  if(counter % 5 == 0)
  {
    sendJson(t, h, fireStatus, yValue);
  }

  if(counter % 20 == 0)
  {
    lcdMessage();
  }
  if(counter==300)
  {
    writeDB(t, h, fireStatus, yValue);
    counter=0;
  }
  
  delay(1000);
  counter++;
}
