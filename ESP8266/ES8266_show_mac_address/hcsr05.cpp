#include"hcsr05.h"

HCSR05::HCSR05(byte trigPin,byte echoPin)
{
  trig=trigPin;
  echo=echoPin;
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
}

float HCSR05::ping()
{
  unsigned long d;
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  d=pulseIn(echo,HIGH,23200);
  return d/58.0;
}

int ping()
{
  return 158;
}
