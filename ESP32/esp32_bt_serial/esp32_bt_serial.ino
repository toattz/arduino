#include<BluetoothSerial.h>

char *pin="9999";
char val;
BluetoothSerial bt;

void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);  
  bt.setPin(pin);
  bt.begin("esp32esp32");

  byte macBT[6];
  esp_read_mac(macBT,ESP_MAC_BT);
  Serial.printf("Bluetooth MAC address:%02X:%02X:%02X:%02X:%02X:%02X\n",macBT[0],macBT[1],macBT[2],macBT[3],macBT[4],macBT[5]);
}

void loop()
{
  if(bt.available())
  {
    val=bt.read();
    switch(val)
    {
      case '0':
        digitalWrite(LED_BUILTIN,HIGH);
        bt.println("turn off the light");
        break;
      case '1':
        digitalWrite(LED_BUILTIN,LOW);
        bt.println("ture on the ligh");
        break;
    }
  }
}
