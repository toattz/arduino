#include<ESP8266WiFi.h>
//#include<ESP8266WebServer.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<WebSocketsServer.h>
#include<FS.h>

#define relayPin D2 //GPIO16

const char* ssid="ZYXEL_1F";
const char* password="C1470A9A";

// Set web server port number to 80
AsyncWebServer server(80);
WebSocketsServer webSocket=WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  if(type==WStype_TEXT)
  {
    //Serial.printf("[%u] get Text: %s\n%zu\n", num, payload,length);
    char s=(char)payload[0];
    int state=atoi(&s);
    if(state==1)
    {
      digitalWrite(relayPin,HIGH);
    }
    else
    {
      digitalWrite(relayPin,LOW);
    }
  }
}

void setup()
{
  pinMode(relayPin,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
 
  Serial.begin(115200);

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.printf("Connecting to %s\n",ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.printf("IP address: %s\n",WiFi.localIP().toString().c_str());
  
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  
  server.on("/state",HTTP_GET,[](AsyncWebServerRequest * req){
    if(digitalRead(relayPin)==HIGH)
    {
      req->send(200,"text/plain",String("1"));
    }
    else
    {
      req->send(200,"text/plain",String("0"));
    }
    });
  server.begin();
  Serial.println("HTTP server started"); 
  
  webSocket.onEvent(webSocketEvent);
  webSocket.begin(); 
  Serial.println("Websock is started");
  
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
}

void loop()
{
  webSocket.loop();
}
