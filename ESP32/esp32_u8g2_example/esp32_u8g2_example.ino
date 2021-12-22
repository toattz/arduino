#include<U8g2lib.h>
volatile int i=0;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);
void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
  //u8g2.setFont(u8g2_font_ncenB08_tr);
  //u8g2.setFont(u8g2_font_cursor_tf);
}

void loop() {
  i=!i;
  if(i==true)
  {
    u8g2.setFont(u8g2_font_ncenB08_tr);
  }
  else
  {
    u8g2.setFont(u8g2_font_cursor_tf);
  }
  // put your main code here, to run repeatedly:
  u8g2.clearBuffer();
  u8g2.drawStr(5,10,"No Hack,");
  u8g2.drawStr(50,30,"No Life!");
  u8g2.sendBuffer();
  delay(1000);
}
