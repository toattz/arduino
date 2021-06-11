const int pin=14;
bool running=true;
volatile int counter=0;
volatile bool state=LOW;
//portMUX_TYPE mux=portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR ISR()
{
  //portENTER_CRITICAL(&mux);
  state=!state;
  counter++;
  digitalWrite(LED_BUILTIN,state);
  //portEXIT_CRITICAL(&mux);
  //Serial.printf("Hello\n");
  printf("Hello\n");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  pinMode(pin,INPUT_PULLUP);
  attachInterrupt(pin,ISR,FALLING);
}

void loop() {
  if(running&&counter>=10)
  {
    running=false;
    detachInterrupt(pin);
    Serial.println("Over");
  }
  // put your main code here, to run repeatedly:

}
