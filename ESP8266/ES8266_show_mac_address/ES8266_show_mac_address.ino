#include<ESP8266WiFi.h>

uint8_t macData[6];
String MacAddress;

void showMac()
{
  Serial.printf("MAC:%s\n",MacAddress.c_str());
}

String getWifiMac()
{
  String tt;
  String t[6];
  //WiFi.status(); //this method must be called for get MAC
  WiFi.macAddress(macData);
  //Serial.printf("\nMACMAC:%s\n",macData
  Serial.printf("\nMAC:");
  for(byte i=0;i<6;i++)
  {
    Serial.print(macData[i],HEX);
    t[i]=String(macData[i],HEX);
    tt+=t[i];
    if(i<5)
      Serial.print("/");
    else
      Serial.println("~");
  }

  for(byte i=0;i<6;i++)
  {
    Serial.print(macData[i]);
    if(i<5)
      Serial.print("/");
    else
      Serial.println("~");
  }
  return tt;
}

void setup()
{
  Serial.begin(115200);
  MacAddress=getWifiMac();
  showMac();  
}

void loop()
{
  
}
