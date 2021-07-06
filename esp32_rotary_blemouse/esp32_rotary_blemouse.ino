#include "rotary_switch.h"
#include <BleKeyboardMouse.h>

BleKeyboardMouse bleKB("多媒體旋鈕");
RotarySwitch rsw(19,21,22);

void setup() 
{
  Serial.begin(115200);
  bleKB.begin();
}

void loop() 
{
  if(bleKB.isConnected())
  {
    switch(rsw.check())
    {
      case RotarySwitch::RELEASED_FROM_PRESS:
        bleKB.write(KEY_MEDIA_PLAY_PAUSE);
        break;
      case RotarySwitch::TURN_RIGHT:
        bleKB.write(KEY_MEDIA_VOLUME_UP);
        break;
      case RotarySwitch::TURN_LEFT:
        bleKB.write(KEY_MEDIA_VOLUME_DOWN);
        break;
    }
  }
}
