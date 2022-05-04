#include<SoftwareSerial.h>

SoftwareSerial mySerial(10,11); //RX,TX  connect to RS232 to USB RX=>TX  TX=>RX
//byte i=0;
void setup()
{
  Serial.begin(115200);
  mySerial.begin(115200);
}

void loop() 
{
  Serial.println(i);
  //mySerial.write("jjjjjjjjjjjj");
  mySerial.write("Testttttttttttt\n");
  /*i=i+1;

  if (i==10)
    i=0;*/
  delay(1000);
}
