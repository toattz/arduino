#include<Arduino_FreeRTOS.h>

#define LED1 6 
#define LED2 7

void taskA(void *pdParam)
{
  pinMode(LED1,OUTPUT);
  while(1)
  {
    digitalWrite(LED1,!digitalRead(LED1));
    vTaskDelay(250/portTICK_PERIOD_MS);
  }
}

void taskB(void *pdParam)
{
  while(1)
  {
    digitalWrite(LED2,!digitalRead(LED2));
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void setup() 
{
  xTaskCreate(taskA,"taskA",100,NULL,1,NULL);
  xTaskCreate(taskB,"taskB",100,NULL,1,NULL);
  vTaskStartScheduler();
}

void loop() {
  // put your main code here, to run repeatedly:

}
