#include <IRremote.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

int RECV_PIN = 2; // 使用數位腳位2接收紅外線訊號
IRrecv irrecv(RECV_PIN); // 初始化紅外線訊號輸入
decode_results results; // 儲存訊號的結構
String strValue="";
String strBits="";
String strDecodeType="";

void setup()
{
  Serial.begin(115200);
  //irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn(); // 啟動接收
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("test");
  lcd.setCursor(0,1);
  lcd.print("test again");
}

void loop() {
  if (irrecv.decode(&results)) { // 接收紅外線訊號並解碼
    strValue="";
    strValue=String(results.value,HEX);
    Serial.print("results value is "+strValue);
    //Serial.print("results value is "+String(results.value,HEX)); // 輸出解碼後的資料
    //Serial.print(results.value, HEX);
    strBits="";
    strBits=String(results.bits);
    Serial.print(", bits is "+strBits);
    //Serial.print(", bits is "+String(results.bits));
    //Serial.print(results.bits);
    strDecodeType="";
    strDecodeType=String(results.decode_type);
    Serial.println(", decode_type is "+strDecodeType);
    //Serial.println(", decode_type is "+String(results.decode_type));
    //Serial.println(results.decode_type);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("value:"+strValue);
    lcd.setCursor(0,1);
    lcd.print("bits:"+strBits);
    irrecv.resume(); // 準備接收下一個訊號
  }
}
