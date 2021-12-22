#include<driver/rtc_io.h>
#define SLEEP_SEC 3

RTC_DATA_ATTR bool LEDstate=0;
gpio_num_t LED=GPIO_NUM_26;


void setup() 
{
  Serial.begin(115200);
  rtc_gpio_init(LED);
  rtc_gpio_set_direction(LED,RTC_GPIO_MODE_OUTPUT_ONLY);
  LEDstate=!LEDstate;
  rtc_gpio_set_level(LED,LEDstate);
  Serial.println("I Am Wake");
  delay(100);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,ESP_PD_OPTION_ON);
  esp_sleep_enable_timer_wakeup(SLEEP_SEC*1000000L);
  Serial.println("ZZZ");
  delay(100);
  esp_deep_sleep_start();

}

void loop() {
  // put your main code here, to run repeatedly:

}
