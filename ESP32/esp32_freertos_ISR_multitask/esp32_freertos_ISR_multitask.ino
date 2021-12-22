#include<U8g2lib.h>

#define ISR_PIN 13
//#define LED 2

SemaphoreHandle_t xSem;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
uint8_t score=0;
volatile bool flashlight=false;

void IRAM_ATTR ISR()
{
  //BaseType_t xHeigherPriorityTaskWoken=pdFALSE;
  Serial.printf("Push button, vaule:%d\n",flashlight);
  flashlight=true;  
  xSemaphoreGiveFromISR(xSem,NULL);
  /*xSemaphoreGiveFromISR(xSem,&xHeigherPriorityTaskWoken);
  if(xHeigherPriorityTaskWoken==pdTRUE)
  {
    portYIELD_FROM_ISR();
  }*/
}

void taskOLED(void *pdParam)
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_inb63_mn);
  String txt;

  while(1)
  {
    if(score<10)
    {
      txt="0"+String(score);
    }
    else
    {
      txt=String(score);
    }

    u8g2.firstPage();
    do{
      u8g2.drawStr(12,63,txt.c_str());      
    }while(u8g2.nextPage());
  }
}

void taskBlink(void *pdParam)
{
  uint8_t counter=0;
  pinMode(LED_BUILTIN,OUTPUT);
  //pinMode(LED,OUTPUT);

  while(1)
  {
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    //digitalWrite(LED,!digitalRead(LED));

    if(!flashlight)
    {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
    else
    {
      if(++counter==10)
      {
        counter=0;
        Serial.printf("counter:%d\n",counter);
        flashlight=false;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void taskScore(void *pdParam)
{
  uint32_t preMS=0;
  while(1)
  {
    if(xSemaphoreTake(xSem,portMAX_DELAY)==pdPASS)
    {
      if(xTaskGetTickCountFromISR()-preMS<1000)
        continue;
      if(++score==100)
      {
        score=0;
      }
      preMS=xTaskGetTickCountFromISR();
    }    
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ISR_PIN,INPUT_PULLUP);
  attachInterrupt(ISR_PIN,ISR,RISING);

  xSem=xSemaphoreCreateBinary();
  if(xSem==NULL)
  {
    Serial.println("Create binary semaphore fail");
  }

  xTaskCreate(taskScore,"task Score",1000,NULL,2,NULL);
  xTaskCreate(taskBlink,"task Blink",1000,NULL,1,NULL);
  xTaskCreate(taskOLED,"task OLED",1500,NULL,1,NULL);
}

void loop()
{
  
}
