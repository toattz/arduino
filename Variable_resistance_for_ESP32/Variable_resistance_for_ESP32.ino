#include <Arduino.h>
#include <analogWrite.h>
int LEDRpin=18;
int LEDGpin=19;
int LEDBpin=21;
void setup() {
  Serial.begin(115200);
  analogWriteResolution(LEDRpin, 12);
  //analogWrite(LEDRpin, 0);  
}
int reading=0, reading4 = 0;
int lightness = 0;
void loop() {
  reading = analogRead(36);//ADC pin
  reading4=analogRead(39);
  Serial.print("Pin 15 : ");
  Serial.println(reading);
  Serial.print("Pin 4 : ");
  Serial.println(reading4 );
  lightness = map(reading, 0, 4095, 0, 255);
  analogWrite(LEDRpin, lightness);
  delay(1000);                                     
}
