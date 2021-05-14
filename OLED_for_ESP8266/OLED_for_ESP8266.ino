#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 3  // GPIO0
Adafruit_SSD1306 OLED(OLED_RESET);
 
void setup()   {
  OLED.begin();
  OLED.clearDisplay();

  //Add stuff into the 'display buffer'
  OLED.setTextWrap(true);
  OLED.setTextSize(1);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0,0);
  OLED.println("automatedhome.party");
  OLED.setCursor(0,7);
  OLED.println("automatedhome.party");
  OLED.setCursor(0,14);
  OLED.println("NEW OLED");
  OLED.setCursor(0,21);
  OLED.println("NEW OLED NEW OLED");
  OLED.display(); //output 'display buffer' to screen  
  //OLED.startscrollleft(0x00, 0x0F); //make display scroll 
} 
 
void loop() {
OLED.clearDisplay();
}
