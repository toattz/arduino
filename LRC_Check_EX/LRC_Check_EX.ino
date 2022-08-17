//Longitudinal Redundancy Check  縱向冗餘校驗  for ASCII mode
char *buf="010304010001"; //ModBus ASCII format code
int num=strlen(buf);

unsigned char GetCheckCode(const char *pSendBuf,int num)
{
  unsigned char byLrc=0;
  char pBuf[2];
  int nData=0;
  int i;

  for(i=0;i<num;i+=2)
  {
    //每兩個需要發送的ASCII碼轉化為十六進位數
    pBuf[0]=pSendBuf[i];
    pBuf[1]=pSendBuf[i+1];
    sscanf(pBuf,"%x",&nData);
    byLrc+=nData;
  }
  byLrc=~byLrc; //相反
  byLrc++;  //加1
  return byLrc;
}

void setup()
{
  Serial.begin(115200);
  Serial.printf("num=%d\n",num);  
  Serial.printf("LRC=%x\n",GetCheckCode(buf,num));
}

void loop()
{
  
}
