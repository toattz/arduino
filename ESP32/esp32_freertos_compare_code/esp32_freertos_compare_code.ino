#define LED1 14
#define LED2 16

unsigned long prevTime1=0;
unsigned long prevTime2=0;


void setup() 
{
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
}

void loop() 
{
  unsigned long now=millis();

  if(now-prevTime1>=250)
  {
    digitalWrite(LED1,!digitalRead(LED1));
    prevTime1=now;
  }

  if(now-prevTime2>=500)
  {
    digitalWrite(LED2,!digitalRead(LED2));
    prevTime2=now;
  }
}
