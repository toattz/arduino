#include<esp_task_wdt.h>


void taskA(void *pdParam)
{
  while(1)
  {
    Serial.println("Task A");
    //printf("Task A");
    vTaskDelay(1);
    //yield();
    //esp_task_wdt_reset();
  }
}

void taskB(void *pdParam)
{
  while(1)
  {
    Serial.println("Task B");
    //printf("Task B");
    //vTaskDelay(1);
  }
}

void setup() 
{
  Serial.begin(115200);
  xTaskCreate(taskA,"taskA",1000,NULL,2,NULL);
  xTaskCreate(taskB,"taskB",1000,NULL,1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
