#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
const int IR_rec = 2;   
char str[10];
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("test");
  lcd.setCursor(0,1);
  lcd.print("test again");
  delay(5000);
  /*char str[]={0xcf,0xb2,0xba,0xdd,0xff};
  lcd.setCursor(0,1);
  lcd.print(str);*/
  pinMode(IR_rec, INPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int IR_status = digitalRead(IR_rec); 
  sprintf(str, "%d", IR_status);
  //itoa=(IR_status, str, 10);
  lcd.clear();
  lcd.print(str);
  Serial.print(str);
  delay(2000);  
}
