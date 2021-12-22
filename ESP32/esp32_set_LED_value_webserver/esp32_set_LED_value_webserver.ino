#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<SPIFFS.h>

#define GREEN 15
#define YELLOW 5
#define BU 4
#define BITS 10

String pwm_val;

const char ssid[]="ZYXEL_1F";
const char password[]="C1470A9A";
unsigned int debounce=0;
const int maxDebounce=200;

AsyncWebServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(BU,INPUT_PULLUP);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0,5000,BITS);
  ledcAttachPin(GREEN,0);
  
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.setHostname("Blanka_ESP32");
  WiFi.begin(ssid,password);

  while (WiFi.status()!=WL_CONNECTED)
    delay(500);

  Serial.printf("IP address:%s\n",WiFi.localIP().toString().c_str());

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/faicon.ico");
  server.on("/pwm",HTTP_GET,[](AsyncWebServerRequest * req){
    AsyncWebParameter* p=req->getParam("val");
    pwm_val=p->value();
    if(digitalRead(YELLOW)==HIGH)
    {
      ledcWrite(0,pwm_val.toInt());
    }
    });
  server.on("/bu",HTTP_GET,[](AsyncWebServerRequest * req ){
    if(digitalRead(YELLOW)==HIGH)
    {
      req->send(200,"text/plain",String("1"));
    }
    else
    {
      req->send(200,"text/plain",String("0"));
    }
    });
  server.begin();
  Serial.println("Server is started");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(BU)==LOW)
  { 
    Serial.println("clcik");
    if(debounce>maxDebounce)
    {
      while(digitalRead(BU)==LOW)
      {
        
      }
      debounce=0;
      digitalWrite(YELLOW,!digitalRead(YELLOW));
      Serial.printf("LED status:%d\n",digitalRead(YELLOW));
    }
    debounce++; 
    
  }

}
