/*
* You need to download mysql library by arudino IDE/Sketch/Include Library/manage Libraries.
* Search "MySQL Connector Arduino" by Dr. Charles Bell, and install it.
*/
#include <HTTPClient.h>
#include <WiFi.h>

//#include <DHT.h>
const char* ssid = "TP-LINK_470A9A_EXT";   // change to your WIFI SSID
const char* password = "C1470A9A";// change to your WIFI Password
String serverName = "http://10.1.1.25/house/aiot.php";
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
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
}

void loop() {
if ((millis() - lastTime) > timerDelay) 
{
  HTTPClient http;
  String serverPath = serverName + "?value=100";   
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
   lastTime = millis();
}
delay(10000);
}
