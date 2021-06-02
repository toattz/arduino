#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include<WebSocketsServer.h>

#define relayPin D2 //GPIO16
#define led D4      //GPIO2
#define USE_SERIAL Serial

const char* ssid="ZYXEL_1F";
const char* password="C1470A9A";
int schedule[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Set web server port number to 80
ESP8266WebServer server(80);
WebSocketsServer webSocket=WebSocketsServer(81);

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

const String scheduleForm="\
<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\
<html>\
  <head>\
  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\">\  
  <title></title>\
  </head>\
  <body>\
    <form>\
     0<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     1<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     2<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     3<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     4<input type=\"checkbox\" onclick=\"mySend()\"/><br>\  
     5<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     6<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     7<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     8<input type=\"checkbox\" onclick=\"mySend()\"/><br>\    
     9<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     10<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     11<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     12<input type=\"checkbox\" onclick=\"mySend()\"/><br>\ 
     13<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     14<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     15<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     16<input type=\"checkbox\" onclick=\"mySend()\"/><br>\ 
     17<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     18<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     19<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     20<input type=\"checkbox\" onclick=\"mySend()\"/><br>\     
     21<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     22<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     23<input type=\"checkbox\" onclick=\"mySend()\"/><br>\
     </form>\                     
  </body>\
</html>\
<script>\
var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);\
function mySend()\
{\
  var schedule=\"\";\
  var checknumbers=document.getElementsByTagName('input');\
  for(var i=0;i<checknumbers.length;i++)\
  {\
    if(checknumbers[i].checked)\
    {\
      var v=1;\
    }\
    else\
    {\
      var v=0;\
    }\
    schedule+=v;\
  }\
  connection.send(schedule);\
}\
</script>\
";


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

void scheduled()
{
  server.send(200,"text/html",scheduleForm);
  
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  if(type==WStype_TEXT)
  {
    USE_SERIAL.printf("[%u] get Text: %s\n%zu\n", num, payload,length);
    for(int i=0;i<length;i++)
    {
      char s=(char)payload[i];
      schedule[i]=atoi(&s);
      USE_SERIAL.printf("schedule[%d]:%d\n",i,schedule[i]);
    } 
  }
}

void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
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
  server.on("/schedule", scheduled);
  Serial.println("HTTP server started"); 
  digitalWrite(relayPin,HIGH);
  delay(500);
  digitalWrite(relayPin,LOW);
  delay(500);
  digitalWrite(relayPin,HIGH);
  delay(500);
  digitalWrite(relayPin,LOW);
  delay(500);
  digitalWrite(relayPin,HIGH);
  delay(500);
  digitalWrite(relayPin,LOW);
  timeClient.begin();
  //timeClient.update();
  //Serial.println(timeClient.getFormattedTime());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  server.handleClient();
}
