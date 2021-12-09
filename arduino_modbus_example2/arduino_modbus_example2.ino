#include<SPI.h>
#include<Ethernet.h>
#include"MgsModbus"

MgsModbug Mb;
int inByte=0;

byte mac[]={0x90,0xA2,0xDA,0x0E,0x94,0xAA};
IPAddress ip(192,168,1,116);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup() 
{
  Serial.begin(115200);
  Serial.println("Serial interface started");

  Ethernet.begin(mac,ip,gateway,subnet);
  Serial.print("My IP Address: ");
  for(bye i=0;i<4;i++)
  {
    Serial.print(Ethernet.localIP()[i],DEC);
    if(i==3)
    {
      Serial.println("");
    }
    else
    {
      Serial.print(".");
    }
  }

  IPAddress serverIP(192,168,

}

void loop() {
  // put your main code here, to run repeatedly:

}
