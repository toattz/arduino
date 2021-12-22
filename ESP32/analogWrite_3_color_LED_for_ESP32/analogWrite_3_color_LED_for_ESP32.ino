#include <Arduino.h>
#include <analogWrite.h>

int LEDRpin=18;
int LEDGpin=19;
int LEDBpin=21;
int brightStep = 1;
int brightness = 0;

void color(int led)
{
  brightness += brightStep;
  if ( brightness == 0 || brightness == 255 ) {
    brightStep = -brightStep;
  }

  analogWrite(led, brightness);

  delay(10);  
}

void setup() {
  // Set resolution for a specific pin
  analogWriteResolution(LEDRpin, 12);
  analogWriteResolution(LEDGpin, 12);
  analogWriteResolution(LEDBpin, 12);
}

void loop() {
  color(LEDRpin);
  //delay(2000);
  //color(LEDGpin);
  //delay(2000);
  //color(LEDBpin);
  //delay(2000);
}
