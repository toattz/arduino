/*#include <Wire.h>
 
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}*/

/*// I2C Scanner
// Written by Nick Gammon
// Date: 20th April 2011
#include <Wire.h>
void setup() { 
    Serial.begin (115200); // Leonardo: wait for serial port to connect 
    while (!Serial) { } 
    Serial.println (); 
    Serial.println ("I2C scanner. Scanning ..."); 
    byte count = 0; 
    Wire.begin(); 
    for (byte i = 8; i < 120; i++) { 
        Wire.beginTransmission (i); 
        if (Wire.endTransmission () == 0) { 
          Serial.print ("Found address: "); 
          Serial.print (i, DEC); 
          Serial.print (" (0x"); 
          Serial.print (i, HEX); 
          Serial.println (")"); 
          count++; 
          delay (1); // maybe unneeded? 
        } // end of good response 
    } // end of for loop 
    Serial.println ("Done."); 
    Serial.print ("Found "); 
    Serial.print (count, DEC); 
    Serial.println (" device(s).");
} // end of setup
void loop() {}*/



#include <Wire.h>

void setup(){
Wire.begin();
Serial.begin(115200);
Serial.println("\nI2C Scanner");
}
void loop(){
byte error, address;
int nDevices;
Serial.println("Scanning...");
nDevices = 0;
for (address = 1; address < 127; address++ ){
// The i2c_scanner uses the return value of
// the Write.endTransmisstion to see if
// a device did acknowledge to the address.
Wire.beginTransmission(address);
error = Wire.endTransmission();
if (error == 0){
Serial.print("I2C device found at address 0x");
if (address < 16)
Serial.print("0");
Serial.print(address, HEX);
Serial.println(" !");
nDevices++;
}else if (error == 4){
Serial.print("Unknow error at address 0x");
if (address < 16)
Serial.print("0");
Serial.println(address, HEX);
}
}
if (nDevices == 0)
Serial.println("No I2C devices found\n");
else
Serial.println("done\n");
delay(5000); // wait 5 seconds for next scan
}
