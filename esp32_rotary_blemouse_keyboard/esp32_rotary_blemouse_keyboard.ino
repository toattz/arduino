#include<BleKeyboardMouse.h>
#include<Keypad.h>
#include"rotary_switch.h"

const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS]={13,12,14,27};
byte colPins[COLS]={26,25,33,32};

RotarySwitch rsw(19,21,22);
BleKeyboardMouse bleKB("媒體控制按鍵");
Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

void setup() 
{
  Serial.begin(115200);
  bleKB.begin();
}

void loop() {
  if(bleKB.isConnected())
  {
    char key=keypad.getKey();
    switch(key)
    {
      case '1':
        bleKB.print("I love DIY");
        bleKB.write(KEY_RETURN);
        break;
      case 'C':
        bleKB.press(KEY_LEFT_CTRL);
        bleKB.press('c');
        delay(100);
        bleKB.releaseAll();
        break;
      case 'D':
        bleKB.press(KEY_LEFT_CTRL);
        bleKB.press('v');
        delay(100);
        bleKB.releaseAll();
        break;
      case '*':
        bleKB.write(KEY_MEDIA_VOLUME_DOWN);
        break;
      case '#':
        bleKB.write(KEY_MEDIA_VOLUME_UP);
        break;
    }

    switch(rsw.check())
    {
      /*case RotarySwitch::RELEASED_FROM_PRESS:
        bleKB.click();
        break;
      case RotarySwitch::TURN_RIGHT:
        bleKB.move(0,0,-1);
        break;
      case RotarySwitch::TURN_LEFT:
        bleKB.move(0,0,1);
        break;*/
    }
  }
}
