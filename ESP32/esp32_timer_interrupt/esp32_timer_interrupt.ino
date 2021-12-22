portMUX_TYPE mux=portMUX_INITIALIZER_UNLOCKED;
volatile bool state=0;
hw_timer_t* timer;

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL(&mux);
  state=!state;
  digitalWrite(LED_BUILTIN,state);
  portEXIT_CRITICAL(&mux);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  timer=timerBegin(0,80,true);
  timerAttachInterrupt(timer,&onTimer,true);
  timerAlarmWrite(timer,1000000,true);
  timerAlarmEnable(timer);
}

void loop() {
  // put your main code here, to run repeatedly:

}
