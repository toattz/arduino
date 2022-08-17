#include<SoftwareSerial.h>
unsigned char cmd[8] = {0x02 ,0x03 ,0x00 ,0x00 ,0x00 ,0x04, 0x44 ,0x0C} ;  // 向表頭發送訊息
//unsigned char cmd[6] = {0x03 ,0x03 ,0x00 ,0x00 ,0x00 ,0x04} ;  // 向表頭發送訊息
uint16_t au16data[16] = {};                                                                                //返回訊息數量
SoftwareSerial mySerial(2,3);
void setup()
{
  Serial.begin(19200) ;                                                        //Arduino串口監控baud-rate (可變更)
  mySerial.begin(19200);
  //Serial2.begin(19200) ;                                                      //Modbus RTU串口通訊baud-rate (可變更)
  Serial.println("RS485 Test Start …..") ;                           //顯示RS485通訊開始
}
 
void loop()
{
  for(int i = 0 ; i <8; i++)
  {
    //Serial2.write(cmd[i]) ;                                               //串口2丟訊息給表頭
    mySerial.write(cmd[i]);
    Serial.print(cmd[i],HEX);
  }
  Serial.println("");
  Serial.println("——Detection start——") ;                //顯示開始讀取
  //if(Serial2.available()>0)
  if(mySerial.available())
  {
    for(int j = 0 ; j <21; j++)
    {
      //while (Serial2.available() >0)
      while(mySerial.available())
      {
        //au16data[j] = Serial2.read();
        au16data[j]=mySerial.read();
        Serial.println(au16data[j],HEX) ;
      }
    }
  }
  delay(1000);                                                              //穩定輸出資訊
}
