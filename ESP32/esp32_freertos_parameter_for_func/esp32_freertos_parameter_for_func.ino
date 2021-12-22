byte LED1=13;
byte LED2=12;

void taskA(void *pdParam)
{
  byte pin=*(byte*)pdParam;
  pinMode(pin,OUTPUT);
  while(1)
  {
    digitalWrite(pin,!digitalRead(pin));
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
void setup() 
{
  xTaskCreate(taskA,"Task LED1",1000,(void*)&LED1,1,NULL);
  xTaskCreate(taskA,"Task LED2",1000,(void*)&LED2,1,NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
