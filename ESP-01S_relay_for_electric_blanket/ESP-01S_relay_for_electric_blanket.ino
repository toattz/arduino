#include<ESP8266WiFi.h>
#include<ESPAsyncWebServer.h>
#include<ESPAsyncTCP.h>
#include<FS.h>
#include<PubSubClient.h>
#include<Ticker.h>
#include<NTPClient.h>
#include<WiFiUdp.h>
//#include<time.h>

//ESP-01S
#define relayPin 15// relay connected to  GPIO0

const char* ssid="TOTOLINK N150RT";
const char* password="0422876333";
const char* mqttServer = "192.168.1.132";
String getCommandTime="None";
const int days_of_leap_year[]={31,29,31,30,31,30,31,31,30,31,30,31};
const int days_of_normal_year[]={31,28,31,30,31,30,31,31,30,31,30,31};

// Set web server port number to 80
AsyncWebServer server(80);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Ticker ticker;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void sendStatus()
{
  if(digitalRead(LED_BUILTIN)==HIGH)
  {
    mqttClient.publish("blanket/relay","off");
  }
  else
  {
    mqttClient.publish("blanket/relay","on");
  }
}

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
  String cmd="";
  for(int i=0;i<noChar;i++)
  {
    cmd+=(char)payload[i];
   
  }
  //Serial.println(topic);
  //Serial.printf("payload:%u\n no boChar:%u\n",payload,noChar);
  Serial.printf("cmd:%s\n",cmd.c_str());

  getCurrentDate();
   
  if(cmd=="off")
  {
    digitalWrite(LED_BUILTIN,HIGH);
    //mqttClient.publish("blanket/relay","off");
  }
  else if(cmd=="on")
  {
    digitalWrite(LED_BUILTIN,LOW);
    //mqttClient.publish("blanket/relay","on");
  }
  else
  {
    //Serial.println("invalid command");
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
  WiFi.mode(WIFI_STA);
  //WiFi.hostname("ESP-01S_CAT_FAN_RELAY_V2");
  WiFi.hostname("小心心的繼電器");
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
      req->send(200,"text/plain",String("0"));
    }
    else
    {
      req->send(200,"text/plain",String("1"));
    }
    });

  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    digitalWrite(relayPin,!digitalRead(relayPin));
    req->send(200);
    });
  server.on("/time",HTTP_GET,[](AsyncWebServerRequest * req){
    req->send(200,"text/plain",getCommandTime);
    });
  server.begin();
  Serial.println("HTTP server started"); 
  
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receivedCMD);
  Serial.println("Start MQTT");

  ticker.attach(5,sendStatus);

  timeClient.begin();
  // 設定時區 *60分 * 60秒，例如:
  // GMT +1 = 3600
  // GMT +8 = 28800  台灣時區
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
   
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
  while(!mqttClient.connected())
  {
    mqttClient.connect("小心心的繼電器");
    mqttClient.subscribe("blanket/dht11");
  }
  mqttClient.loop();
}
