#ifndef HCSR05_H
#define HCSR05_H
#include<Arduino.h>

class HCSR05{
  private:
    byte trig;
    byte echo;
    
  public:
    HCSR05(byte trigPin,byte echoPin);
    /*{
      trig=trigPin;
      echo=echoPin;
      pinMode(trig,OUTPUT);
      pinMode(echo,INPUT);
    }*/

    float ping();
    /*{
      unsigned long d;
      digitalWrite(trig,HIGH);
      delayMicroseconds(10);
      digitalWrite(trig,LOW);
      d=pulseIn(echo,HIGH,23200);
      return d/58.0;
    }*/
};
#endif

int ping();
