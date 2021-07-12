TaskHandle_t handleB;
long counter=0;

void taskA(void *pdParam)
{
  while(1)
  {
    Serial.println("Task A");
    counter++;
    if(counter==100)
    {
      UBaseType_t taskPriority=uxTaskPriorityGet(handleB);
      vTaskPrioritySet(handleB,taskPriority+2);
      //vTaskPrioritySet(handleB,3);
    }
    yield();
  }
}

void taskB(void *pdParam)
{
  while(1)
  {
    Serial.println("Task BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
    if(counter==200)
    {
      vTaskDelete(handleB);
    }
    yield();
  }
}

void setup()
{
  Serial.begin(115200);
  xTaskCreate(taskA,"Task A",1000,NULL,2,NULL);
  xTaskCreate(taskB,"Task B",1000,NULL,1,&handleB);
}

void loop() {
  // put your main code here, to run repeatedly:

}
