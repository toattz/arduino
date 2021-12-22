#include "rotary_switch.h"

RotarySwitch rsw(19,21,22);

void setup() 
{
  Serial.begin(115200); 
}

void loop() 
{
  switch(rsw.check())
  {
    case RotarySwitch::RELEASED_FROM_PRESS:
      Serial.println("按一下");
      break;
    case RotarySwitch::PRESSING:
      Serial.println("按著");
      break;
    case RotarySwitch::LONG_PRESSED:
      Serial.println("長按");
      break;
    case RotarySwitch::TURN_RIGHT:
      Serial.println("往上轉動");
      break;
    case RotarySwitch::TURN_LEFT:
      Serial.println("往下轉動");
      break;
  }

}
