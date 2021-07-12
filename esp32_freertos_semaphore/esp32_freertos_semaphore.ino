SemaphoreHandle_t xSem=xSemaphoreCreateMutex();

void printJob(char *str)
{
  xSemaphoreTake(xSem,portMAX_DELAY);
  Serial.println(str);
  xSemaphoreGive(xSem);
}

void myTask(void *pdParam)
{
  char *pStr=(char*)pdParam;
  while(1)
  {
    printJob(pStr);
    vTaskDelay(1);
  }
}

void setup()
{
  Serial.begin(115200);
  xTaskCreate(myTask,"task1",1500,(void*)"==========",1,NULL);
  xTaskCreate(myTask,"task2",1500,(void*)"++++++++++",1,NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
