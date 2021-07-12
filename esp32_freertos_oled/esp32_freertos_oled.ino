#include<U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);

void taskOLED(void *pdParam)
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  int num=0;
  String txt;

  while(1)
  {
    txt="Counter:"+String(num++);
    u8g2.firstPage();
    do{
      u8g2.drawStr(0,15,txt.c_str());
      
    }while(u8g2.nextPage());
    Serial.printf("taskOLED core at:%u\n",xPortGetCoreID());
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void setup() 
{
  Serial.begin(115200);
  //u8g2.begin();
  xTaskCreate(taskOLED,"taskOLED",1500,NULL,1,NULL);
  //xTaskCreatePinnedToCore(taskOLED,"taskOLED",1500,NULL,1,NULL,1);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Serial.printf("main core at:%u\n",xPortGetCoreID());
  vTaskDelay(1000/portTICK_PERIOD_MS);

}
