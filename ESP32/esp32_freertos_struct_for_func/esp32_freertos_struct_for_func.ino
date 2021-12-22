typedef struct data{
  byte pin;
  int ms;
}LED;

LED LED1={13,500};
LED LED2={12,250};

void taskA(void *pdParam)
{
  LED *led=(LED*)pdParam;
  byte pin=led->pin;
  int ms=led->ms;
  pinMode(pin,OUTPUT);
  while(1)
  {
    digitalWrite(pin,!digitalRead(pin));
    vTaskDelay(ms/portTICK_PERIOD_MS);
  }
}
void setup() {
  xTaskCreate(taskA,"Task LED1",1000,(void*)&LED1,1,NULL);
  xTaskCreate(taskA,"Task LED2",1000,(void*)&LED2,1,NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}
