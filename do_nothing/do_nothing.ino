#include<time.h>


typedef struct data
{
  byte pin;
  int ms;
}LED1;

char * weekDays[7]={"日","一","二","三","四","五","六"};

void setup()
{
  Serial.begin(115200);
  /*LED1 led;
  LED1 *pLed=&led;
  LED1 LED2={20,22};
  
  (*pLed).pin=2;
  (*pLed).ms=5;
  Serial.printf("led.pin:%d\nled.ms:%d\n",pLed->pin,pLed->ms);
  Serial.printf("LED2.pin:%d\nLED2.ms:%d\n",(&LED2)->pin,(&LED2)->ms);
  Serial.printf("LED2.pin:%d\nLED2.ms:%d\n",LED2.pin,LED2.ms);*/
  time_t today=time(NULL);
  //struct tm xmas={0,0,0,25,12,2025-1900};
  struct tm xmas;
  xmas.tm_year=125;
  xmas.tm_mon=12;
  xmas.tm_mday=25;
  (&xmas)->tm_hour=10;
  (&xmas)->tm_min=10;
  (&xmas)->tm_sec=10;
  Serial.printf("before setup:%u\n",today);
  today=mktime(&xmas);
  Serial.printf("after setup:%u\n",today);
  struct tm * localtm=localtime(&today);
  Serial.printf("asctime:%s\n",asctime(localtm));
  Serial.println(localtm,"%Y/%m/%d %H:%M:%S");
  Serial.printf("ctime:%s\n",ctime(&today));
  char * wday= weekDays[localtm->tm_wday];
  Serial.printf("星期%s\n",wday);
}

void loop()
{
  
}
