#include<Servo.h>
#define SW_PIN 7
#define SERVO_PIN 0
Servo servo;
int flag=1;
void setup() {
  // put your setup code here, to run once:
  pinMode(SW_PIN,INPUT_PULLUP);
  servo.attach(SERVO_PIN,500,2400);
  servo.write(90);
}
void loop() {
  // put your main code here, to run repeatedly:
  
  //if(digitalRead(SW_PIN)==0)
  {
   // if(flag==0)
    {
      servo.write(180);
      flag=1;
    }
   // else
    {
      delay(2000);
      servo.write(0);
      flag=0;
    }
  }
  delay(2000);  
}
