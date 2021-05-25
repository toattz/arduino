#include<Servo.h>
#define SW_PIN 7
#define SERVO_PIN 8

Servo servo;

void setup() 
{
  pinMode(SW_PIN,INPUT_PULLUP);
  servo.attach(SERVO_PIN,500,2400);
  servo.write(90);
}

void loop()
{ 
  bool sw=digitalRead(SW_PIN);

  if(sw==0)
  {
    servo.write(150);
    delay(2000);
    servo.write(90);
    delay(1000);
    servo.write(0);
  }
  // put your main code here, to run repeatedly:

}
