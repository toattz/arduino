#define ETH 0

#include<SPI.h>
#if ETH
#include<Ethernet.h>
#else
#include<ESP8266WiFi.h>
#endif
//download MgsModbus from https://myarduinoprojects.com/modbus.html
//download modpoll https://www.modbusdriver.com/modpoll.html
#include"MgsModbus.h"

MgsModbus Mb;
int inByte=0;

#if ETH
byte mac[]={0x90,0xA2,0xDA,0x0E,0x94,0xB5};
#endif
void setup() 
{
  Serial.begin(115200);
  Serial.println("Serial interface started");
#if ETH
  Ethernet.begin(mac);
  
  Serial.print("IP Address:");
  for(byte thisByte=0;thisByte<4;thisByte++)
  {
    Serial.print(Ethernet.localIP()[thisByte],DEC);
    if(thisByte==3)
    {
      Serial.println("");
    } 
    else
    {
      Serial.print(".");
    }
  }
#else
  WiFi.hostname("Modbus_ESP8266");
  WiFi.begin("ZYXEL_1F","C1470A9A");
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
#endif 

//print menu
  Serial.println("0 - print the first 12 words of the MbData space");
  Serial.println("1 - fill MbData with 0x0000 hex");
  Serial.println("2 - fill MbData with 0x00,0x01,0x02,0x03");
  Serial.println("3 - fill MbData with 0xAAA hex");
  /*Serial.println("0 - print the first 12 words of the MbData space");
  Serial.println("1 - FC 3 - read 6 registers from the the slave to mbData[0..5]");
  Serial.println("2 - FC 16 - write 6 registers MbData[6..11] to server");*/
}

void loop() 
{
  if(Serial.available())
  {
    inByte=Serial.read();

    if(inByte=='0')
    {
      for(int i=0;i<12;i++)
      {
        Serial.print("address: ");
        Serial.print(i);
        Serial.print(" Data: ");
        Serial.println(Mb.MbData[i],HEX);
      }
    }
    else if(inByte=='1')
    {
      for(int i=0;i<12;i++)
      {
        Mb.MbData[i]=0x0000;
      }
    }
    else if(inByte=='2')
    {
      Mb.MbData[0]=0x0000;
      Mb.MbData[1]=0x0001;
      Mb.MbData[2]=0x0002;
      Mb.MbData[3]=0x0003;
    }
    else if(inByte=='3')
    {
      for(int i=0;i<12;i++)
      {
        Mb.MbData[i]=0xAAA;
      }
    }
    else
    {
      Serial.println("Nono");
    }
  }
  Mb.MbsRun();
}
