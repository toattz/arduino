#define LED1 14
#define LED2 16

void taskA(void *pvParam)
{
  pinMode(LED1,OUTPUT);
  while(1)
  {
    digitalWrite(LED1,!digitalRead(LED1));
    //vTaskDelay(pdMS_TO_TICKS(250));
    vTaskDelay(250);
  }
}

void taskB(void *pvParam)
{
  pinMode(LED2,OUTPUT);
  while(1)
  {
    digitalWrite(LED2,!digitalRead(LED2));
    //vTaskDelay(500/portTICK_PERIOD_MS);
    vTaskDelay(500);
  }
}

void setup() 
{
  xTaskCreate(taskA,"taskA",1000,NULL,1,NULL);
  xTaskCreate(taskB,"taskB",1000,NULL,1,NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
