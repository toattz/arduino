#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("test");
  lcd.setCursor(0,1);
  lcd.print("test again");
  delay(5000);
  char str[]={0xcf,0xb2,0xba,0xdd,0xff};
  lcd.setCursor(0,1);
  lcd.print(str);
}

void loop() {
  // put your main code here, to run repeatedly:

}
