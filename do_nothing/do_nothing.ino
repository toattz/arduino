const byte swPin=2;
const byte ledPin=13;
volatile bool state=LOW;

void swISR()
{
  state=!state;
  digitalWrite(ledPin,state);
  Serial.println(state);
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
  pinMode(swPin,INPUT_PULLUP);
  attachInterrupt(0,swISR,CHANGE);
}

void loop()
{
  
}
