SemaphoreHandle_t xSem=xSemaphoreCreateBinary();

void taskA(void *pdParam)
{
  while(1)
  {
    xSemaphoreTake(xSem,portMAX_DELAY);
    Serial.println("AAAAAAAAAAAAAA");
    xSemaphoreGive(xSem);
    vTaskDelay(1);
  }
}

void taskB(void *pdParam)
{
  while(1)
  {
    xSemaphoreTake(xSem,portMAX_DELAY);
    Serial.println("BBBBBBBBBBBBBB");
    xSemaphoreGive(xSem);
    vTaskDelay(1);
  }
  
}

void taskC(void *pdParam)
{
  while(1)
  {
    xSemaphoreTake(xSem,portMAX_DELAY);
    Serial.println("CCCCCCCCCCCCCC");
    xSemaphoreGive(xSem);
    vTaskDelay(1);
  }  
}
void setup()
{
  Serial.begin(115200);
  xTaskCreate(taskA,"task A",1500,NULL,1,NULL);
  xTaskCreate(taskB,"task B",1500,NULL,1,NULL);
  xTaskCreate(taskC,"task C",1500,NULL,1,NULL);
  xSemaphoreGive(xSem);

}

void loop() {
  // put your main code here, to run repeatedly:

}
