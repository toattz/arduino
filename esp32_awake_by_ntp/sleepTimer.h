#ifdef SLEEP_TIMER_H
#define SLEEP_TIMER_H
#include<time.h>

class SleepTimer{
  private:
  	uint8_t tolerance;
  	const char * ntpServer="pool.ntp.org";

  	void goSleep(uint32_t sec)
  	{
  	  esp_sleep_enable_timer_wakeup(sec*1000000L);
  	  Serial.printf("Wake seconds:%u\n", sec);
  	  Serial.println("ZZZ");
  	  delay(50);
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
  	}

  	int8_t start(uint8_t mn,uint8_t sec,void(*ptFunc)())
  	{

  	}
};


#endif
