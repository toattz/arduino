#include<SoftwareSerial.h>

SoftwareSerial mySerial(2,3); //RX TX
char val;
byte count=0;
//String vals="";
char line[16];
void setup()
{
  memset(line,'\0',sizeof(line));
  Serial.begin(115200);
  mySerial.begin(9600); //降速避免亂碼
  Serial.println("Start system host");
  mySerial.println("Start system host");

}

void loop()
{
  if(Serial.available())
  {
    val=Serial.read();
    //Serial.write(val);
    //Serial.println(String("Count:")+count);
    if(count<16&&val!=10)
    {
      //vals+=String(char(val));
      line[count]=char(val);
      //Serial.println(String("read char:")+val);
      count=count+1;
    }
    else if(val==10)
    {
      //Serial.println(String("sizeof val:")+sizeof(vals));     
      //Serial.println(vals);
      //mySerial.println(vals);
      Serial.println(line);
      mySerial.println(line);
      memset(line,'\0',sizeof(line));
      //vals="";
      count=0;
    }
  }

  if(mySerial.available())
  {
    Serial.write(mySerial.read());
  }
}
