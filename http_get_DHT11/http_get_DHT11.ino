/*
* You need to download mysql library by arudino IDE/Sketch/Include Library/manage Libraries.
* Search "MySQL Connector Arduino" by Dr. Charles Bell, and install it.
*/
#include <HTTPClient.h>
#include <WiFi.h>
#include <DHT.h>
//#include <NTPClient.h>
//#include <WiFiUdp.h>
#define DHTPIN 15   //DHT11 DATA 数据引脚
#define DHTTYPE DHT11  //选择的类型
const char* ssid = "TP-LINK_470A9A_EXT";   // change to your WIFI SSID
const char* password = "C1470A9A";// change to your WIFI Password
String serverName = "http://10.1.1.44/aiot.php";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
char temptostr[10]; 
char humitostr[10]; 
DHT dht(DHTPIN, DHTTYPE);
//WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.setHostname("ESP32");
  WiFi.begin(ssid, password);
  
  //try to connect to WIFI 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); 
  dht.begin();

  /*// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);*/
}

void loop() {
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
  
//if ((millis() - lastTime) > timerDelay) 
{
  HTTPClient http;
  String serverPath = serverName + "?temp=" + temptostr + "&humi=" + humitostr;   
  // Your Domain name with URL path or IP address with path
  
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
  //lastTime = millis();
  }
  /*while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);*/
  delay(298000);
}
