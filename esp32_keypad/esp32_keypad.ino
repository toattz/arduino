#include<Keypad.h>

#define KEY_ROWS 4
#define KEY_COLS 4

char keymap[KEY_ROWS][KEY_COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[KEY_COLS]={26,25,33,32};
byte rowPins[KEY_ROWS]={13,12,14,27};

Keypad myKeypad=Keypad(makeKeymap(keymap),rowPins,colPins,KEY_ROWS,KEY_COLS);
void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  char key=myKeypad.getKey();
  if(key)
  {
    Serial.println(key);
  }

}
