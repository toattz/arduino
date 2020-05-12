#include<Keypad.h>
const byte ROWS=4;
const byte COLS=4;
char keys[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}};

byte rowPins[ROWS]={5,6,7,8};
byte colPins[COLS]={9,10,11,12};
Keypad keypad=Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if(key != NO_KEY)
  {
      Serial.print(key);
  }
}
