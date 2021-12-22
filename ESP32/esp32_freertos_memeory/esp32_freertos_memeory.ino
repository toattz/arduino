#define LED1 13
#define LED2 12

TaskHandle_t handleA;
uint16_t stackSize=700;

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
  pinMode(LED2,OUTPUT);
  while(1)
  {
    digitalWrite(LED2,!digitalRead(LED2));
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void setup()
{
  Serial.begin(115200);
  Serial.printf("Stack size:%u bytes\n",ESP.getHeapSize());
  Serial,printf("Before create task A:%u bytes\n",ESP.getFreeHeap());
  xTaskCreate(taskA,"Task A",stackSize,NULL,1,&handleA);
  xTaskCreate(taskB,"Task B",stackSize,NULL,1,NULL);
  Serial.printf("After create task A:%u bytes\n",ESP.getFreeHeap());
}

void loop() 
{
  UBaseType_t m=stackSize-uxTaskGetStackHighWaterMark(handleA);
  Serial.printf("Task A used %u bytes\n",m);
  vTaskDelay(1000);

}
