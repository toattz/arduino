#include <WiFi.h>
#include<ESPAsyncWebServer.h>
#include<LiquidCrystal_I2C.h>
#include<TridentTD_LineNotify.h>
#include<DHT.h>
#include<HTTPClient.h>
#include<WiFiClient.h>
#include<SPIFFS.h>
#include<ESPmDNS.h>


#define WIFISSID      "ZYXEL_1F"
#define WIFIPASSWORD  "C1470A9A"
#define MDNS_NAME     "demo"
//LED
#define REDLED  18
#define YELLOWLED 4
#define GREENLED 2
#define REDLEDDEMOPIN 13  //button pin
#define BITS 10 // 取樣為10位元
//DHT11
#define DHTTYPE           DHT11
#define DHTPIN            19
//rotary encoder
#define CLK_PIN 14
#define DT_PIN 27
#define SW_PIN 26
//line message
#define LINE_TOKEN        "V5xuingKhVKe8Hd5jRmwIypMPwxqFyp3trLmCIRiVbd"

char *st[]={"INIT",
  "NETWORK_INFO",
  "DHT11_INFO",
  "SEND_LINE_MESSAGE_TEST",
  "RESTART_DEVICE",
  "REDLEDDEMO",
  "YELLOWLEDDEMO",
  "GREENLEDDEMO",
  "REDLEDDEMOCHANGED",
  "YELLOWLEDDEMOCHANGED",
  "GREENLEDDEMOCHANGED"};

typedef enum:int8_t{
  INIT,
  NETWORK_INFO,
  DHT11_INFO,
  SEND_LINE_MESSAGE_TEST,
  RESTART_DEVICE,
  REDLEDDEMO,
  YELLOWLEDDEMO,
  GREENLEDDEMO,
  REDLEDDEMOCHANGED,
  YELLOWLEDDEMOCHANGED,
  GREENLEDDEMOCHANGED
    }SystemStatus;

bool redLEDDemo=true;
bool yellowLEDDemo=true;
bool greenLEDDemo=false;
uint16_t maxDebounce=500;
uint16_t debounce=0;
int16_t greenLEDVal=0;
bool greenLEDValReverse=false;
bool sendLine=false;
bool sendDHT=false;
bool restartDevice=false;
bool prev=LOW;
bool now=LOW;
float t=0;//temperature
float h=0;///humidity
uint16_t dhtCount=0;

LiquidCrystal_I2C lcd(0x27,20,4);
AsyncWebServer server(80);
DHT dht(DHTPIN, DHTTYPE);
SystemStatus status=NETWORK_INFO;
SystemStatus prevStatus=INIT;

void checkLEDDemoStatus()
{
  if(status==REDLEDDEMO)
    status=REDLEDDEMOCHANGED;

  if(status==YELLOWLEDDEMO)
  {
    status=YELLOWLEDDEMOCHANGED;
  }
  
  if(status==GREENLEDDEMO)
  {
    status=GREENLEDDEMOCHANGED;
  }
}

void getRedLEDDemo(bool focus)
{
  lcd.setCursor(0,1);
  if(focus==true)
  {
    if(redLEDDemo==true)
    {
      lcd.print("Red LED Demo:on\x07F");
    }
    else
    {
      lcd.print("Red LED Demo:off\x07F");
    }
  }
  else
  {
    if(redLEDDemo==true)
    {
      lcd.print("Red LED Demo:on");
    }
    else
    {
      lcd.print("Red LED Demo:off");
    }
  }
}

void getYellowLEDDemo(bool focus)
{
  lcd.setCursor(0,2);
  if(focus==true)
  {
    if(yellowLEDDemo==true)
    {
      lcd.print("Yellow LED Demo:on\x07F");
    }
    else
    {
      lcd.print("Yellow LED Demo:off\x07F");
    }
  }
  else
  {
    if(yellowLEDDemo==true)
    {
      lcd.print("Yellow LED Demo:on");
    }
    else
    {
      lcd.print("Yellow LED Demo:off");
    }
  }
}

void getGreenLEDDemo(bool focus)
{
  lcd.setCursor(0,3);
  if(focus==true)
  {
    if(greenLEDDemo==true)
    {
      lcd.print("Green LED Demo:on\x07F");
    }
    else
    {
      lcd.print("Green LED Demo:off\x07F");
    }
  }
  else
  {
    if(greenLEDDemo==true)
    {
      lcd.print("Green LED Demo:on");
    }
    else
    {
      lcd.print("Green LED Demo:off");
    }
  }
}

void taskLCD(void *pvParam)
{
  Wire.begin();
  //Wire.setClock(5000);//set wire clock to prevent LCD garbled 5kMhz
  Wire.setClock(7500);//set wire clock to prevent LCD garbled 5kMhz
  lcd.init();
  lcd.backlight();
  while(1)
  {
    //Serial.println(st[status]);
    if(status!=prevStatus)
    {
      lcd.clear();
      delay(50);
      switch(status)
      {
        case NETWORK_INFO:
          lcd.setCursor(0,0);
          lcd.print("Network Info");
          lcd.setCursor(0,1);
          lcd.printf("AP:%s",WIFISSID);
          lcd.setCursor(0,2);
          lcd.printf("IP:%s",WiFi.localIP().toString().c_str());
          lcd.setCursor(0,3);
          lcd.printf("mDNS:%s.local",MDNS_NAME);
          prevStatus=NETWORK_INFO;
          break;

        case DHT11_INFO:       
          lcd.setCursor(0,0);
          lcd.print("DHT11 Info");
          lcd.setCursor(0,1);
          lcd.printf("Temperature:%s\x0DF",String(t,2));
          lcd.setCursor(18,1);
          lcd.print("C");
          lcd.setCursor(0,2);
          lcd.printf("Humidity:%s%%",String(h,2));
          lcd.setCursor(0,3);
          lcd.print("Send Data\x07F");
          prevStatus=DHT11_INFO;
          break;

        case SEND_LINE_MESSAGE_TEST:
          lcd.setCursor(0,0);
          lcd.print("Send Line Message");
          lcd.setCursor(0,3);
          lcd.print("Send Test Message\x07F");
          prevStatus=SEND_LINE_MESSAGE_TEST;
          break;

        case RESTART_DEVICE:
          lcd.setCursor(0,0);
          lcd.print("Restart Device");
          lcd.setCursor(0,3);
          lcd.print("Reboot\x07F");
          prevStatus=RESTART_DEVICE;
          break;

        case REDLEDDEMO:
        case REDLEDDEMOCHANGED:
          lcd.setCursor(0,0);
          lcd.print("LED Demo");
          getRedLEDDemo(true);
          getYellowLEDDemo(false);
          getGreenLEDDemo(false);
          if(status==REDLEDDEMOCHANGED)
          {
            status=REDLEDDEMO;
          }
          prevStatus=REDLEDDEMO;
          break;

        case YELLOWLEDDEMO:
        case YELLOWLEDDEMOCHANGED:
          lcd.setCursor(0,0);
          lcd.print("LED Demo");
          getRedLEDDemo(false);
          getYellowLEDDemo(true);
          getGreenLEDDemo(false);
          if(status==YELLOWLEDDEMOCHANGED)
          {
            status=YELLOWLEDDEMO;
          }
          prevStatus=YELLOWLEDDEMO;
          break;

        case GREENLEDDEMO:
        case GREENLEDDEMOCHANGED:
          lcd.setCursor(0,0);
          lcd.print("LED Demo");
          getRedLEDDemo(false);
          getYellowLEDDemo(false);
          getGreenLEDDemo(true);
          if(status==GREENLEDDEMOCHANGED)
          {
            status=GREENLEDDEMO;
          }
          prevStatus=GREENLEDDEMO;
          break;
      }
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
  } 
}

void taskGreenLED(void *pvParam)
{
  while(1)
  {
    if(greenLEDDemo==true)
    {
      ledcWrite(1,greenLEDVal);
      if(greenLEDValReverse==false)
      {
        greenLEDVal+=1;
      }
      else
      {
        greenLEDVal-=1;
      }
      
      if(greenLEDVal>1023)
      {
        greenLEDVal=1023;
        greenLEDValReverse=true;
      }
      else if(greenLEDVal<0)
      {
        greenLEDVal=0;
        greenLEDValReverse=false;
      }
    }
    else
    {
      ledcWrite(1,0);
    }
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);

  /*Wire.begin();
  Wire.setClock(5000);*/
  /*lcd.init();
  lcd.backlight();
  lcd.print("Init System");*/

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  //rotary encoder
  pinMode(CLK_PIN,INPUT_PULLUP);
  pinMode(DT_PIN,INPUT_PULLUP);
  pinMode(SW_PIN,INPUT_PULLUP);
  prev=digitalRead(CLK_PIN);

  //red LED setting
  pinMode(REDLEDDEMOPIN,INPUT_PULLUP);
  pinMode(REDLED,OUTPUT);
  digitalWrite(REDLED,LOW);

  //yellow LED setting
  pinMode(YELLOWLED,OUTPUT);
  digitalWrite(YELLOWLED,LOW);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0,5000,BITS);
  ledcAttachPin(YELLOWLED,0);
  
  //green LED setting
  pinMode(GREENLED,OUTPUT); 
  digitalWrite(GREENLED,LOW);
  ledcSetup(1,5000,BITS);
  ledcAttachPin(GREENLED,1);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("ESP32_page2");
  WiFi.begin(WIFISSID,WIFIPASSWORD);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  if(!MDNS.begin(MDNS_NAME))
  {
    Serial.println("Error setting up MDNS responder!");
    while (1)
    {
      delay(1000);
    }
  }

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.serveStatic("/js",SPIFFS,"/www/js/");
  server.serveStatic("/css",SPIFFS,"/www/css/");

  server.on("/ap",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",WIFISSID);
    });
  
  server.on("/ip",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",WiFi.localIP().toString().c_str());
    });

  server.on("/mdns",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",MDNS_NAME+String(".local"));
    });

  server.on("/temperature",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",String(t,2));
    });

  server.on("/humidity",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",String(h,2));
    });

  server.on("/redLEDDemo",HTTP_GET,[](AsyncWebServerRequest * req){
    if(redLEDDemo==true)
    {
      req->send(200,"text/plain",String(1));
    }
    else
    {
      req->send(200,"text/plain",String(0));
    }
    });

  server.on("/yellowLEDDemo",HTTP_GET,[](AsyncWebServerRequest * req){
    if(yellowLEDDemo==true)
    {
      req->send(200,"text/plain",String(1));
    }
    else
    {
      req->send(200,"text/plain",String(0));
    }
    });

  server.on("/greenLEDDemo",HTTP_GET,[](AsyncWebServerRequest * req){
    if(greenLEDDemo==true)
    {
      req->send(200,"text/plain",String(1));
    }
    else
    {
      req->send(200,"text/plain",String(0));
    }
    });
    
  server.on("/DHT11",HTTP_POST,[](AsyncWebServerRequest * req){
    sendDHT=true;
    req->send(200);
    });

  server.on("/line",HTTP_POST,[](AsyncWebServerRequest * req){
    sendLine=true;
    req->send(200);
    });
    
  server.on("/restartDevice",HTTP_POST,[](AsyncWebServerRequest * req){
    restartDevice=true;
    req->send(200);
    });

  server.on("/redLEDDemo",HTTP_POST,[](AsyncWebServerRequest * req){
    redLEDDemo=!redLEDDemo;
    checkLEDDemoStatus();
    req->send(200);
    });

  server.on("/yellowLEDDemo",HTTP_POST,[](AsyncWebServerRequest * req){
    yellowLEDDemo=!yellowLEDDemo;
    checkLEDDemoStatus();
    req->send(200);
    });

  server.on("/greenLEDDemo",HTTP_POST,[](AsyncWebServerRequest * req){
    greenLEDDemo=!greenLEDDemo;
    checkLEDDemoStatus();
    req->send(200);
    });
  
  server.begin();

  MDNS.setInstanceName("for demo only, No question!!!!");
  MDNS.addService("http","tcp",80);
  dht.begin();

  LINE.setToken(LINE_TOKEN);
  
  xTaskCreate(taskGreenLED,"taskGreenLED",1000,NULL,1,NULL);
  xTaskCreate(taskLCD,"taskLCD",4000,NULL,1,NULL);
}

void loop() 
{
  //read DHT11 in loop() is stable
  if(dhtCount==8192)
  {  t=dht.readTemperature();
     h=dht.readHumidity();
     dhtCount=0;
  }
  dhtCount++;
  
  if(digitalRead(REDLEDDEMOPIN)==LOW && redLEDDemo==true)
  {
    //Serial.println("digitalRead(REDLEDDEMOPIN)==LOW");
    if(debounce<maxDebounce)
    {
      debounce++;
    }
    else
    {
      while(digitalRead(REDLEDDEMOPIN)==LOW){}
      digitalWrite(REDLED,!digitalRead(REDLED));
      debounce=0;
    }
  }
  else if(redLEDDemo==false)
  {
    digitalWrite(REDLED,LOW);
  }

  if(yellowLEDDemo==true)
  {
    //int val =1023-analogRead(A0);
    int val=analogRead(A0);
    ledcWrite(0,val);
  }
  else
  {
    ledcWrite(0,0);
  }

  //rotary encoder
  now=digitalRead(CLK_PIN);
  if(now!=prev)
  {
    if(digitalRead(DT_PIN)!=now)
    {
      //Serial.println("turn right");
      switch(status)
      {
        case NETWORK_INFO:
          status=DHT11_INFO;
          break;

        case DHT11_INFO:
          status=SEND_LINE_MESSAGE_TEST;
          break;

        case SEND_LINE_MESSAGE_TEST:
          status=RESTART_DEVICE;
          break;

        case RESTART_DEVICE:
          status=REDLEDDEMO;
          break;

        case REDLEDDEMO:
          status=YELLOWLEDDEMO;
          break;
          
        case YELLOWLEDDEMO:
          status=GREENLEDDEMO;
          break;

        case GREENLEDDEMO:
          status=NETWORK_INFO;
          break;
      }
    }
    else
    {
      //Serial.println("turn left");
      switch(status)
      {
        case NETWORK_INFO:
          status=REDLEDDEMO;
          break;

        case REDLEDDEMO:
          status=RESTART_DEVICE;
          break;

        case RESTART_DEVICE:
          status=SEND_LINE_MESSAGE_TEST;
          break;

        case SEND_LINE_MESSAGE_TEST:
          status=DHT11_INFO;
          break;

        case DHT11_INFO:
          status=NETWORK_INFO;
          break;

        case GREENLEDDEMO:
          status=YELLOWLEDDEMO;
          break;

        case YELLOWLEDDEMO:
          status=REDLEDDEMO;
          break;
      }
    }
  }
  prev=now;

  //Serial.printf("SW_PIN:%d\n",digitalRead(SW_PIN));
  if(digitalRead(SW_PIN)==LOW)
  {
    //Serial.println("digitalRead(SW_PIN)==LOW");
    if(debounce<maxDebounce)
    {
      debounce++;
    }
    else
    {
      while(digitalRead(SW_PIN)==LOW){}
      switch(status)
      {
        case DHT11_INFO:
          sendDHT=true;
          break;

        case SEND_LINE_MESSAGE_TEST:
          sendLine=true;
          break;

        case RESTART_DEVICE:
          restartDevice=true;
          break;

        case REDLEDDEMO:
          status=REDLEDDEMOCHANGED;
          redLEDDemo=!redLEDDemo;
          break;

        case YELLOWLEDDEMO:
          status=YELLOWLEDDEMOCHANGED;
          yellowLEDDemo=!yellowLEDDemo;
          break;

        case GREENLEDDEMO:
          status=GREENLEDDEMOCHANGED;
          greenLEDDemo=!greenLEDDemo;
          break;
      }
      debounce=0;
    }
  }
  if(sendLine==true)
  {
    //Serial.println("send line");
    LINE.notify("ESP32 demo");
    LINE.notifySticker(3,240);        // ส่ง Line Sticker ด้วย PackageID 3 , StickerID 240
    LINE.notifySticker("Hello",1,2);  // ส่ง Line Sticker ด้วย PackageID 1 , StickerID 2  พร้อมข้อความ
    LINE.notifyPicture("https://preview.ibb.co/j6G51n/capture25610417181915334.png");
    LINE.notifyPicture("จตุธาตุ","https://www.fotoaparat.cz/storage/pm/09/10/23/670915_a5351.jpg");
    LINE.notifyPicture("測試用","http://picsum.photos/300/300?random=100");
    sendLine=false;
  }

  if(sendDHT==true)
  {
    //Serial.println("send DHT");
    String url="http://toattz.pythonanywhere.com/getDHT11/"+String(t,2)+"/"+String(h,2)+"/";
    WiFiClient client;
    HTTPClient httpClient;
    httpClient.begin(client,url); 
    httpClient.GET();
    httpClient.end();
    sendDHT=false;
  }

  if(restartDevice==true)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Device Rebooting...");
    ESP.restart();
  }
}
