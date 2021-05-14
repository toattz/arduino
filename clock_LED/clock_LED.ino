#include<DS1302.h>
#include<TM1637Display.h>
// DS1302 初始化設定
//DS1302 rtc(2, 3, 4);

// 設定 TM1637 接腳
#define CLK 9
#define DIO 8

TM1637Display display(CLK, DIO);
/*boolean colon = true ;

String dw = "";
String hh = "";
String mm = "";
String ss = ""; 
float t = 0; */
int i =0;
  
void setup()
{
  // 設定時鐘執行模式，取消寫入保護
//  rtc.halt(false);
//  rtc.writeProtect(false);
  
  // Setup Serial connection
  Serial.begin(115200);
  display.setBrightness(0xA);
  
// 第一次設定寫入 DS1302 RTC時鐘，之後可以加上註解
//  rtc.setDOW(SUNDAY);         // 設定每週星期幾？
//  rtc.setTime(20, 16, 30);     // 設定24小時時間 20:16:30 
//  rtc.setDate(19, 3, 2017);   // 設定日期(日, 月, 年)
}

void loop()
{
  // 取得星期幾
  //Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // 取得日期
  //Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // 取得時間
  //dw = rtc.getTimeStr();
  //Serial.println(dw);
  
  //hh = dw.substring(0,2);          // 時數
  //mm = dw.substring(3,5);          // 分鐘數
  //ss = dw.substring(6,8);          // 秒數 
  if(i<=9999)
  {
  // 顯示四位數中間的冒號
    uint8_t segto;
    int value = 1000;
  // 顯示 時：分
  //int  t =  hh.toInt()*100  + mm.toInt();
  // 顯示 分：秒
  //  int t =  mm.toInt() *100 +ss.toInt();
    segto = 0x80 | display.encodeDigit((i / 100)%10);
    display.setSegments(&segto, 1, 1);
    delay(50);

  // 顯示時間
    display.showNumberDec(i, true);
    delay(50);
    i=i+1;
  }
  else
  {
    i=0;
  }
}
