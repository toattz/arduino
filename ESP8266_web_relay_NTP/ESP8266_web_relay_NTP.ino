#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include<NTPClient.h>
#include<WiFiUdp.h>

const char* ssid="TOTOLINK_470A9A";
const char* password="C1470A9A";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Set web server port number to 80
ESP8266WebServer server(80);
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Variable to store the HTTP request
String header;

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>POST plain text to /postplain/</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postplain/\">\
      <input type=\"text\" name=\'{\"hello\": \"world\", \"trash\": \"\' value=\'\"}\'><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
    <h1>POST form data to /postform/</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <input type=\"text\" name=\"hello\" value=\"world\"><br>\
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

//const String switch 


int getHour()
{
  int h=0;
  char h1=timeClient.getFormattedTime()[0];
  char h2=timeClient.getFormattedTime()[1];
  Serial.print("h1:");
  Serial.println(h1);
  Serial.print("h2:");
  Serial.println(h2);
  h=(atoi(&h1)*10)+atoi(&h2);
  Serial.print("h:");
  Serial.println(h);
  if(h>=24)
  {
    h=h-24;
  }
  Serial.print("h:");
  Serial.println(h);
  return h;
}

void handleRoot() {
  //digitalWrite(led, 1);
  server.send(200, "text/html", postForms);
  //digitalWrite(led, 0);
}

void handleSchedule()
{
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  char hour1=timeClient.getFormattedTime()[0];
  char hour2=timeClient.getFormattedTime()[1];
  Serial.println(hour1);
  Serial.println(hour2);
  Serial.println(getHour());
  server.send(200,"text/html","schedule"); 
  server.send(200,"text/html","nnnnnnnn");  
}

void setup() {
  Serial.begin(115200);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  server.on("/", handleRoot);
  server.on("/schedule",handleSchedule);
  Serial.println("HTTP server started");
  timeClient.begin();
  
}

void loop() {
  
  server.handleClient();
}
