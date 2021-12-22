#include<SPIFFS.h>
#include<ArduinoJson.h>
#include"switch.h"

#define BITS 10
#define STEPS 20

const byte LED=22;
const byte SW_UP=19;
const byte SW_DW=18;
const byte CHANGE_VAL=1024/STEPS;
const char * CONFIG_FILE="/config.json";
const uint16_t SAVE_INTERVAL=5000;
bool clicked=false;
uint32_t clickTime=0;
uint16_t pwmVal=0;

Switch upSW(SW_UP,LOW,true);
Switch downSW(SW_DW,LOW,true);

void lightUp()
{
  if((pwmVal+CHANGE_VAL)<=1023)
  {
    pwmVal+=CHANGE_VAL;
    Serial.println(pwmVal);
    ledcWrite(0,pwmVal);
  }
}

void lightDown()
{
  if((pwmVal-CHANGE_VAL)>=0)
  {
    pwmVal-=CHANGE_VAL;
    Serial.println(pwmVal);
    ledcWrite(0,pwmVal);
  }
}

void writeJson(uint16_t n)
{
  StaticJsonDocument<100> doc;
  doc["light"]=n;
  doc["desc"]="紀錄亮度";

  String jsonStr;
  serializeJson(doc,jsonStr);
  Serial.printf("JSON String:%s\n",jsonStr.c_str());

  File file=SPIFFS.open(CONFIG_FILE,"w");

  if(!file)
  {
    Serial.printf("Can not open file:%s\n",CONFIG_FILE);  
    return;
  }
  else
  {
    file.println(jsonStr.c_str());
    file.close();
  }
}

uint16_t readJson()
{
  String jsonStr;
  File file=SPIFFS.open(CONFIG_FILE,"r");

  if(!file||file.size()==0)
  {
    Serial.printf("Can not open file:%s\n",CONFIG_FILE);  
    return 0;    
  }
  else
  {
    while(file.available())
    {
      jsonStr=file.readStringUntil('\n');
    }
    Serial.printf("File content:%s\n",jsonStr.c_str());
    file.close();
  }
  StaticJsonDocument<100> doc;

  deserializeJson(doc,jsonStr);
  uint16_t light=doc["light"];
  String desc=doc["desc"];
  Serial.printf("light:%u\ndesc:%s\n",light,desc.c_str());
  return light;


  
}
void setup() 
{
  Serial.begin(115200);
  pinMode(LED,OUTPUT);

  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  ledcSetup(0,5000,BITS);
  ledcAttachPin(LED,0);

  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    while(1)
    {
      delay(10);
    }
  }

  pwmVal=readJson();
  ledcWrite(0,pwmVal);

}

void loop()
{
  switch (upSW.check())
  {
    case RELEASED_FROM_PRESS:
    case PRESSING:
      clicked=true;
      clickTime=millis();
      lightUp();
      break;  
  }

  switch(downSW.check())
  {
    case RELEASED_FROM_PRESS:
    case PRESSING:
      clicked=true;
      clickTime=millis();
      lightDown();
      break;
  }

  if(clicked&&(millis()-clickTime)>=SAVE_INTERVAL)
  {
    clicked=false;
    writeJson(pwmVal);
  }
}
