#include<U8g2lib.h>

#define BITS 10
#define ADC_PIN A4

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
QueueHandle_t queue;

void taskOLED(void *pdParam)
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  int num=0;
  String txt="Vds value:";

  while(1)
  {
    if(xQueueReceive(queue,&num,portMAX_DELAY)==pdPASS)
    //if(xQueueReceive(queue,&num,250/portTICK_PERIOD_MS)==pdPASS)
    //if(xQueueReceive(queue,&num,pdMS_TO_TICK(250))==pdPASS)
    {
      u8g2.firstPage();
      do{
        u8g2.drawStr(0,10,(txt+num).c_str());
      }while(u8g2.nextPage());
      Serial.printf("Get Cds:%d\n",num);
    }
    else
    {
      Serial.println("\no Cds value\n");
    }
  }
}

void taskCDS(void *pdParam)
{
  int adc=0;
  while(1)
  {
    adc=analogRead(ADC_PIN);
    if(xQueueSend(queue,&adc,portMAX_DELAY)==pdPASS)
    {
      Serial.printf("Write data %d to queue\n",adc);
    }
    else
    {
      Serial.printf("Write data to queue is fail\n",adc);
    }
    vTaskDelay(250/portTICK_PERIOD_MS);
  }
}
void setup() 
{
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);
  queue=xQueueCreate(5,sizeof(int));

  if(queue==NULL)
  {
    Serial.println("Create queue fail");
    return;
  }
  
  xTaskCreate(taskCDS,"task CDS",3000,NULL,1,NULL);
  xTaskCreate(taskOLED,"task OLED",1500,NULL,1,NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
