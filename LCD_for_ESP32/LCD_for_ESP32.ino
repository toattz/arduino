//#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);//set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){ 
  lcd.init();// initialize the lcd with SDA and SCL pins
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("by EasyIoT");
  lcd.setCursor(0,2);
  lcd.print("01234567890123456789");
  lcd.setCursor(0,3);
  lcd.print("01234567890123456789");
}
void loop(){
}

/*#include <Arduino.h>
//#include <analogWrite.h>
#include<LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x06,16,2);
void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("test");
 
}
int reading = 0;
int lightness = 0;
void loop() {
  reading = analogRead(15);//ADC pin
  Serial.println(reading);
  //lightness = map(reading, 0, 4095, 0, 255);
  //analogWrite(LEDRpin, lightness);
  delay(1000);                                     
}*/
/*
 * Scanning...
I2C device found at address 0x06
I2C device found at address 0x0B
I2C device found at address 0x0C
I2C device found at address 0x0D
I2C device found at address 0x0E
I2C device found at address 0x15
I2C device found at address 0x1A
I2C device found at address 0x1B
I2C device found at address 0x1F
done
*/
