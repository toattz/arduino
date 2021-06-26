#include<qrcode.h>

void setup() 
{
  Serial.begin(115200);

  uint32_t dt=millis();

  QRCode qrcode;
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  qrcode_initText(&qrcode,qrcodeData,3,ECC_LOW,"hello");

  dt=millis()-dt;
  Serial.printf("Make QRCode took:%dms\n\n",dt);

  for(byte y=0;y<qrcode.size;y++)
  {
    for(byte x=0;x<qrcode.size;x++)
    {
      if(qrcode_getModule(&qrcode,x,y))
      {
        Serial.print("\u25A0\u25A0");
      }
      else
      {
        Serial.print("\u25A1\u25A1");
      }
    }
    Serial.println("");
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
