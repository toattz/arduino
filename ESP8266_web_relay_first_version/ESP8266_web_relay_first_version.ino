#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

#define relayPin D2 //GPIO16
//#define led D4      //GPIO2
const char* ssid="ZYXEL_3F";
const char* password="C1470A9A";

// Set web server port number to 80
ESP8266WebServer server(80);

const String onForms = "<html>\
  <head>\
    <meta charset=\"utf-8\">\
    <title>ESP8266 Web Server Switch handling</title>\
  </head>\
  <body>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/\">\
      <div style=\"position:absolute;left:50%;top:50%;margin-left:-150px;margin-top:-150px;\">\
        <h1>狀態:開</h1><br>\
        <input type=\"hidden\" name=\"sw\" value=\"0\">\
        <input type=\"submit\" value=\"關\" style=\"width:120px;height:40px;font-size:20px;\">\
      </div>\
    </form>\
  </body>\
</html>";

const String offForms = "<html>\
  <head>\
    <meta charset=\"utf-8\">\
    <title>ESP8266 Web Server Switch handling</title>\
  </head>\
  <body>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/\">\
      <div style=\"position:absolute;left:50%;top:50%;margin-left:-150px;margin-top:-150px;\">\
        <h1>狀態:關</h1><br>\
        <input type=\"hidden\" name=\"sw\" value=\"1\">\
        <input type=\"submit\" value=\"開\" style=\"width:120px;height:40px;font-size:20px;\">\
      </div>\
    </form>\
  </body>\
</html>";

void handleRoot()
{
  if(server.method()==HTTP_POST)
  {
    //server.args==> args count
    //server.argName()==>plain
    //server.arg()==> sw=1 in here
    char sw=(char)server.arg(0)[3];
    //Serial.print("sw:");
    //Serial.println(atoi(&sw));
    if(atoi(&sw)==1)
    {
      //Serial.println("enter 1");
      digitalWrite(relayPin,HIGH);
    }
    if(atoi(&sw)==0)
    {
      //Serial.println("enter 0");
      digitalWrite(relayPin,LOW);
    }
  }
  //Serial.print("relay:");
  //Serial.println(digitalRead(relayPin));
  if(digitalRead(relayPin)==1)
  {
    server.send(200, "text/html", onForms);
  }
  else
  {
    server.send(200, "text/html", offForms);
  }
}

void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
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
  Serial.printf("IP address: %s\n",WiFi.localIP().toString().c_str());
  Serial.printf("Wifi RSSI: %d\n",WiFi.RSSI());
  
  server.begin();
  server.on("/", handleRoot);
  Serial.println("HTTP server started"); 
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  delay(500);
  digitalWrite(LED_BUILTIN,HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.printf("LED_BUILTIN:%d\n",LED_BUILTIN);
}

void loop()
{
  server.handleClient();
}
