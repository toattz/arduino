//Cyclial Redundancy Check 循環冗餘校驗  for RTU mode
unsigned short getCrc(unsigned char *data,int len)
{
  unsigned short crc=0xffff;
  int i,j;
  unsigned char LSB;

  for(i=0;i<len;i++)
  {
    crc^=data[i]; //xor運算
    for(j=0;j<8;j++)
    {
      LSB=crc&1; //測試bit0
      crc=crc>>1;  //右移一位
      if(LSB)
      {
        crc^=0xA001; //xor運算
      }
    }
  }
  return ((crc&0xff00)>>8)|((crc&0x00ff)<<8);  //高低位元組交換
}

void setup()
{
  Serial.begin(115200);
  unsigned char tmp[]={0x01,0x03,0x21,0x02,0x00,0x02}; //ModBus RTU碼
  Serial.printf("%x\n",getCrc(tmp,6));
}

void loop()
{
  // put your main code here, to run repeatedly:

}
