#include <WiFi.h> //for ESP32
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define windPin 36
#define quakePin 39


String serverName = "http://10.1.1.102/set4.php";
const char* ssid = "TP-LINK_470A9A";//請查無線網路名稱
const char* password = "C1470A9A"; //請查密碼
const char* mqttServer = "10.1.1.102"; //請確認
int counter=0;
WiFiClient wifiClient1;
PubSubClient mqttClient1(wifiClient1);
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oledDisplay(int wind, int quake)
{
  OLED.clearDisplay();
  OLED.setCursor(0,0);
  OLED.print("WIND : ");
  OLED.println(wind);
  OLED.setCursor(0,7);
  OLED.print("EARTH QUAKE : ");
  OLED.println(quake);
  OLED.display();
}

void writeDB(int wind, int quake)
{
  char windtostr[10];
  char quaketostr[10];
  HTTPClient http;
  itoa(wind, windtostr, 10);
  itoa(quake, quaketostr, 10);
  String serverPath = serverName + "?wind=" + windtostr + "&quake=" + quaketostr;   
  /*sprintf(str, "");
  sprintf(str, "http://10.1.1.16/set4.php?wind=%d&quake=$d&x=%d&y=%d&z=%d",windValue, quakeValue, xValue, yValue, zValue);*/
  // Your Domain name with URL path or IP address with path
  Serial.println(serverPath);
  http.begin(serverPath.c_str());
 
  // Send HTTP GET request
  int httpResponseCode = http.GET();
     
  if (httpResponseCode>0)
  {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
   }
   else 
   {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
   }
    // Free resources
  http.end();
}

void sendJson(int wind, int quake)
{ 
  char str[200];
  sprintf(str,"");
  sprintf(str, "{\"id\":4,\"wind\":%d,\"quake\":%d}", wind, quake);
  mqttClient1.publish("ack", str);  
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar) {
  /*char *response;
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
  mqttClient1.publish("ack", response);*/
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
  Serial.println("Start MQTT");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  OLED.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  OLED.clearDisplay();
  OLED.setTextWrap(true);
  OLED.setTextSize(2);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println(WiFi.localIP());
  OLED.display();
  delay(10000);
  OLED.setTextSize(1);
}
void loop()
{
  int windValue, quakeValue;
  
  windValue = analogRead(windPin);
  quakeValue = analogRead(quakePin);

  while( !mqttClient1.connected()) {
    mqttClient1.connect("ESP32");
    mqttClient1.subscribe("command");
   }
  mqttClient1.loop();

  Serial.print("wind : ");
  Serial.println(windValue);  
  Serial.print("quake : ");
  Serial.println(quakeValue);
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  oledDisplay(windValue, quakeValue);
  if(counter % 5 == 0)
  {
    sendJson(windValue, quakeValue);
  }

  if(counter==300)
  {
    writeDB(windValue, quakeValue);
    counter=0;
  }
  delay(1000);
  counter++;
}
