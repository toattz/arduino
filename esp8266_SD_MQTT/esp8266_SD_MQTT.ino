#include<ESP8266WiFi.h>
#include<SD.h>
#include<PubSubClient.h>
#include<Ticker.h>
#include<NTPClient.h>
#include<WiFiUdp.h>
#include<FS.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>

//Wemos D1
//pino ligado ao CS do módulo SD Card
#define CS_PIN  D10

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
String getCommandTime="None";
const int days_of_leap_year[]={31,29,31,30,31,30,31,31,30,31,30,31};
const int days_of_normal_year[]={31,28,31,30,31,30,31,31,30,31,30,31};
const char* mqttServer = "192.168.1.132";
Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);
AsyncWebServer server(80);

void getCurrentDate()
{
  timeClient.update();
  //getCommandTime=timeClient.getFormattedTime();
  unsigned long epochTime = timeClient.getEpochTime();
  //1970,1971,1973為平年, 1972為閏年,以此類推
  //計算年
  unsigned int remainder_of_4_years=epochTime%((365*24*60*60*3)+(366*24*60*60));  //計算每四年為一單位的餘數
  unsigned int years=int(((epochTime-remainder_of_4_years)/((365*24*60*60*3)+(365*24*60*60)))*4)+1970;
  int isLeapYear=0;
  //判斷是否潤年  將epochTime減至未滿一年
  if(remainder_of_4_years<(365*24*60*60))//未滿一年
  {
    epochTime=remainder_of_4_years;
    isLeapYear=0;
  }
  else if(remainder_of_4_years>(365*24*60*60)&&remainder_of_4_years<(365*24*60*60*2))//未滿兩年
  {
    epochTime=remainder_of_4_years-(365*24*60*60);
    isLeapYear=0;
    years=years+1;
  }
  else if(remainder_of_4_years>(365*24*60*60*2)&&remainder_of_4_years<(365*24*60*60*2)+(366*24*60*60))//未滿三年
  {
    epochTime=remainder_of_4_years-(365*24*60*60*2);
    isLeapYear=1;
    years=years+2;
  }
  else//未滿四年
  {
    epochTime=remainder_of_4_years-((365*24*60*60*2)+(366*24*60*60));
    isLeapYear=0;
    years=years+3;
  }

  unsigned int months=0; //計算月
  if(isLeapYear==1)
  {
    while(epochTime>(days_of_leap_year[months]*24*60*60))
    {
      epochTime=epochTime-(days_of_leap_year[months]*24*60*60);
      months=months+1;
    }
  }
  else
  {
    while(epochTime>(days_of_normal_year[months]*24*60*60))
    {
      epochTime=epochTime-(days_of_normal_year[months]*24*60*60);
      months=months+1;
    }    
  }

  unsigned int date=0;
  while(epochTime>(24*60*60))
  {
    epochTime=epochTime-(24*60*60);
    date+=1;
  }

  unsigned int hour=0;
  while(epochTime>(60*60))
  {
    epochTime=epochTime-(60*60);
    hour+=1;
  }
  
  unsigned int minute=0;
  while(epochTime>60)
  {
    epochTime=epochTime-60;
    minute+=1;  
  }
  
  unsigned int sec=epochTime;
  //Serial.printf("years:%d\nmonth:%d\ndate:%d\nhour:%d\nminute:%d\nsec:%d\n",years,months+1,date+1,hour,minute,sec);
  getCommandTime=String(years)+"/"+String(months+1)+"/"+String(date+1)+" "+String(hour)+":"+String(minute)+":"+String(sec);
}

void receivedCMD(char* topic, byte* payload, unsigned int noChar)
{
  getCurrentDate();
  String cmd="";
  for(int i=0;i<noChar;i++)
  {
    cmd+=(char)payload[i];  
  }

  Serial.printf("date:%s IP:%s cmd:%s\n",getCommandTime.c_str(),WiFi.localIP().toString().c_str(),cmd.c_str());

  File file=SD.open("log.txt","a");
  file.printf("date:%s IP:%s cmd:%s\n",getCommandTime.c_str(),WiFi.localIP().toString().c_str(),cmd.c_str());
  file.close();
}

void ledBlink()
{
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);

  if(!SPIFFS.begin())
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  Serial.print("\r\nWaiting for SD card to initialise...");
  
  if (!SD.begin(CS_PIN))
  {         // CS接在D10
    Serial.println("Initialising failed!");
    return;
  }
  Serial.println("SD card Initialisation completed");

  WiFi.mode(WIFI_STA);
  WiFi.hostname("RX_DATA");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  ticker.attach(1,ledBlink);
  
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");

  timeClient.begin();
  // 設定時區 *60分 * 60秒，例如:
  // GMT +1 = 3600
  // GMT +8 = 28800  台灣時區
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.on("/log",HTTP_GET,[](AsyncWebServerRequest *req){
    //String logs="";
    File file=SD.open("log.txt","r");
    if(!file)
    {
      Serial.println("open file fail");
      return;
    }
    else
    {
      Serial.println("open file success");
    }
    /*while(file.available())
    {
      logs+=(char)file.read();
    }
    file.close();
    Serial.println(logs.c_str());*/
    req->send(file,"/log.txt","text/xhr");
  });
  server.begin();  
}

void loop()
{
  while(!mqttClient.connected())
  {
    mqttClient.connect("WEMOS");
    mqttClient.subscribe("command");
  }
  mqttClient.loop();

}
