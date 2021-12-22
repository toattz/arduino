#include<U8g2lib.h>
#include<DHT.h>

#define DHTTYPE DHT11
#define BITS 10


typedef struct sensorStruct{
  byte pin;
  float value;
}sensor_t;

const byte ADC_PIN=A4;
const byte DHTPIN=16;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
DHT dht(DHTPIN,DHTTYPE);
QueueHandle_t queue;

void taskOLED(void *pdParam)
{
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  sensor_t data;
  float cdsVal=0,tempVal=0;
    
  while(1)
  {
    xQueueReceive(queue,&data,portMAX_DELAY);
    switch(data.pin)
    {
      case ADC_PIN:
        cdsVal=(int)data.value;
        Serial.printf("cdsVal:%f\n",cdsVal);
        break;
      case DHTPIN:
        tempVal=data.value;
        Serial.printf("tempVal:%f\n",tempVal);
        break;
    }
    
    u8g2.firstPage();
    do{
      u8g2.drawStr(0,10,(String("cds:")+(int)cdsVal).c_str());
      u8g2.drawStr(0,25,("temp:"+String(tempVal,2)).c_str());
    }while(u8g2.nextPage());
  }
}

void taskCDS(void *pdParam)
{
  int adc=0;
  sensor_t cds;
  cds.pin=ADC_PIN;

  while(1)
  {
    adc=analogRead(ADC_PIN);
    Serial.printf("get ADC:%d\n",adc);
    cds.value=float(adc);
    Serial.printf("cds.value:%f\n",cds.value);
    xQueueSend(queue,&cds,portMAX_DELAY);
    vTaskDelay(250/portTICK_PERIOD_MS);
  }
}

void taskTemp(void *pdParam)
{
  dht.begin();
  sensor_t temp;
  temp.pin=DHTPIN;

  while(1)
  {
    temp.value=dht.readTemperature();
    Serial.printf("task temp.value:%f\n",temp.value);
    xQueueSend(queue,&temp,portMAX_DELAY);
    vTaskDelay(250/portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  

  analogSetAttenuation(ADC_11db);
  analogSetWidth(BITS);

  queue=xQueueCreate(10,sizeof(sensor_t));
  if(queue==NULL)
  {
    Serial.println("Create queue fail");
    return;
  }
  else
  {
    Serial.println("Create queue success");
  }

  xTaskCreate(taskCDS,"task CDS",2000,NULL,1,NULL);
  xTaskCreate(taskTemp,"task Temperature",2000,NULL,1,NULL);
  xTaskCreate(taskOLED,"task OLED",2000,NULL,1,NULL);
}

void loop() 
{
 
  // put your main code here, to run repeatedly:

}
