#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<WebSocketsServer.h>
#include<SPIFFS.h>
#include<ArduinoJson.h>
#define LED1 15

char *ssid="ZYXEL_1F";
char *password="C1470A9A";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onSocketEvent(AsyncWebSocket *server,
                   AsyncWebSocketClient *client,
                   AwsEventType type,
                   void *arg,
                   uint8_t *data,
                   size_t len)
{
  switch(type)
  {
    case WS_EVT_CONNECT:
      Serial.printf("User %u from %s is connected\n",client->id(),client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("User %u from %s is disconneted\n",client->id(),client->remoteIP().toString().c_str());
      break;
    case WS_EVT_ERROR:
      Serial.printf("User %u is send:%s\n",client->id(),(char *)data);
      break;
    case WS_EVT_DATA:
      Serial.printf("User %u is wrong:%s\n",client->id(),(char *)data);
      const size_t capacity=JSON_OBJECT_SIZE(2)+20;
      DynamicJsonDocument doc(capacity);
      deserializeJson(doc,data);

      const char *device=doc["device"];
      int val=doc["val"];
      if(strcmp(device,"LED")==0)
      {
        if(val!=2)
        {
          digitalWrite(LED1,val);
        }
        
        const size_t capacity=JSON_OBJECT_SIZE(2);
        DynamicJsonDocument doc(capacity);
        doc["device"]="LED";
        if(digitalRead(LED1)==HIGH)
        {
          doc["val"]="1";
        }
        else
        {
          doc["val"]="0";
        }
        
        char data[30];
        serializeJson(doc,data);
        ws.textAll(data);
      }
      break;
    /*case WS_EVT_PONG:
      Serial.printf("PONG\n");
      break;*/
  }
}

void notifyClients()
{
  const size_t capacity=JSON_OBJECT_SIZE(2);
  DynamicJsonDocument doc(capacity);
  doc["device"]="LDR";
  doc["val"]=analogRead(A4);

  char data[30];
  serializeJson(doc,data);
  ws.textAll(data);
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(LED1,OUTPUT);
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.printf("SPIFFS is broken");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.hostname("ESP32_WebSocket");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.onNotFound([](AsyncWebServerRequest * req){
    req->send(404,"text/plain","Not found");
    });
  
  ws.onEvent(onSocketEvent);
  server.addHandler(&ws);
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  ws.cleanupClients();
  notifyClients();
  delay(5000);

}
