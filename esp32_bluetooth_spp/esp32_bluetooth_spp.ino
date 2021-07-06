#include<BluetoothSerial.h>

BluetoothSerial bt;
byte clientAddr[6]={0x24,0x6F,0x28,0x15,0xE8,0x92};
String clientName="esp32esp32";
bool connected;
void setup() 
{
  Serial.begin(115200);
  bt.begin("controller",true);
  Serial.println("Ready to connect");
  connected=bt.connect(clientAddr);

  if(connected)
  {
    Serial.println("the connection is ready");
  }
  else
  {
    while(!bt.connected(10000))
    {
      Serial.println("unable to connect");
    }
  }

}

void loop() 
{
  if(Serial.available())
  {
    bt.write(Serial.read());  
  }
  if(bt.available())
  {
    Serial.write(bt.read());
  }
}
