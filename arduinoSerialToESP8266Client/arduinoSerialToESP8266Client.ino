//Host:Arduino Uno
//Client:ESP8266 Wemos D1 R1
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

byte count=0;
char val;
//String vals="";
bool first=true;
char line1[16];
char line2[16];

SoftwareSerial mySerial(12,13); //RX TX  D6 D7
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() 
{
  memset(line1,'\0',sizeof(line1));
  lcd.init();                      // initialize the lcd
  Serial.begin(115200);
  mySerial.begin(9600); //降速避免亂碼
  lcd.backlight();
  lcd.print("//mlwmlw.org");
  lcd.setCursor(0,1);
  lcd.print("ESP8266!!1234");
  Serial.printf("Start system client\n");
  mySerial.printf("Start system client\n");
}

void loop() 
{
  if(mySerial.available())
  {
    val=mySerial.read();
    //Serial.println(val);
    if(count<16&&val!=10&&val!=13)
    {
      //Serial.printf("String(char(val)):%s count:%d val:%d\n",String(char(val)),count,val);
      //vals+=String(char(val));
      line1[count]=char(val);
      count=count+1;
    }
    else if(val==10)
    {
      Serial.println(line1);
      //Serial.println(vals);
      lcd.clear();
      lcd.setCursor(0,0);
      //char buf[16];
      //Serial.printf("count:%d\n",count);
      //Serial.printf("val length:%d\n",vals.length());
      //Serial.printf("val lastIndex:%d\n",vals.lastIndexOf('\n'));
      //Serial.printf("sizeof vals:%d\n",sizeof(vals));
      //vals.toCharArray(buf,count+1);
      //lcd.print(buf);
      lcd.print(line1);
      mySerial.println(String("Got string \"")+String(+line1)+"\".");
      //mySerial.println(line1);
      if(first==true)
      {
        first=false;
      }
      else
      {
        lcd.setCursor(0,1);
        lcd.print(line2);
      }

      memcpy(line2,line1,sizeof(line2));
      memset(line1,'\0',sizeof(line1));
      //vals="";
      count=0;
    }
  }
  
}
