#ifndef SLEEP_TIMER_H
#define SLEEP_TIMER_H
#include<time.h>

class SleepTimer
{
  private:
    uint8_t tolerance;
    const char * ntpServer="pool.ntp.org";

    void goSleep(uint32_t sec)
    {
      esp_sleep_enable_timer_wakeup(sec*1000000L);
      Serial.printf("喚醒秒數:%u\n",sec);
      Serial.println("ZZZ");
      esp_deep_sleep_start();
    }

  public:
    SleepTimer(uint8_t sec)
    {
      tolerance=sec;
    }

    void init(uint16_t utcOffset=28800,uint8_t daylightOffset=0)
    {
      configTime(utcOffset,daylightOffset,ntpServer);
      Serial.println("configTime");
    }

    int start(uint8_t mn,uint8_t sec,void (*ptFunc)())
    {
      if(mn<0||mn>60)
      {
        return -2;
      }
      if(sec<0||sec>60)
      {
        return -3;
      }

      struct tm now;
      if(!getLocalTime(&now))
      {
        return -1;
      }

      int8_t mnNow=now.tm_min;
      int8_t secNow=now.tm_sec;
      Serial.printf("設定時間 %d分 %d秒\n",mn,sec);
      Serial.printf("現在時間 %d分 %d秒\n",mnNow,secNow);

      if(mn==0)
      {
        mn=60;
      }
      if(sec==0)
      {
        sec=60; 
      }

      int16_t diffTime=(mn*60+sec)-(mnNow*60-secNow);
      Serial.printf("相差秒數 %d\n",diffTime);

      if(abs(diffTime)<=tolerance)
      {
        (*ptFunc)();
        goSleep(diffTime*3600);
      }
      else
      {
        if(diffTime<0)
        {
          diffTime+=3600;
        }
        goSleep(diffTime);
      }   
      return 1;
    }
};
#endif
