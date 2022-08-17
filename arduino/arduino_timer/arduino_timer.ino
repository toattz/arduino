#include<LiquidCrystal_I2C.h> 
//#include<Wire.h> 
#include<Ticker.h>
#include <IRremote.h>

//for arduino uno 
/*
#define BUZZER_PIN 7
#define CLK_PIN 8 
#define DT_PIN 9
#define SW_PIN 10
*/

//for pro micro
#define BUZZER_PIN 8
#define CLK_PIN 14  
#define DT_PIN 16
#define SW_PIN 15
#define IR_PIN 7

int counter=0;
int prevCounter=0;
int plus=0;
int minus=0;
int page=0;
const int maxDebounce=500;
uint32_t millisEnd=0; // for timer
uint32_t millisNow=0; // for timer
uint32_t millisRemain=0;
uint32_t debounce=0;
uint32_t millisAction=0;
uint32_t millisLCDSaver=0;
bool now=0;
bool prev=0;
String countStr="";

//IR data
String strValue="";
String strBits="";
String strDecodeType="";

void taskLCD();
void taskCount();
void taskLCDSaver();
void taskPrint();

LiquidCrystal_I2C lcd(0x27,16,2);  //設定LCD位置0x27,設定LCD大小為16*2
IRrecv irrecv(IR_PIN); // 初始化紅外線訊號輸入
decode_results results; // 儲存訊號的結構
Ticker tickLCD(taskLCD,200);
Ticker tickCount(taskCount,1000);
Ticker tickLCDSaver(taskLCDSaver,1000);
Ticker tickPrint(taskPrint,60000);

typedef enum{
  /////////////for timer
  NONE,         //0
  SETUP_TIMER,  //1
  INIT,         //2
  INIT_YES,     //3
  INIT_NO,      //4
  SETUP,        //5
  COUNTING,     //6
  IR_MODE,      //7  /////////////for IR
  IR_DATA_PAGE1,//8
  IR_DATA_PAGE2, //9
  DEVICE_UPTIME,
  LCD_SAVER
}SystemStatus;
SystemStatus status=SETUP_TIMER;
SystemStatus prevStatus=SETUP_TIMER;

String countDeviceUptime()
{
  uint32_t deviceUptime=floor(millis()/1000);
  uint32_t deviceUptimeSecond=deviceUptime%60;
  uint32_t deviceUptimeMinute=((deviceUptime-deviceUptimeSecond)%(60*60))/60;
  uint32_t deviceUptimeHour=((deviceUptime-(deviceUptimeMinute*60)-deviceUptimeSecond))/(60*60);
  //uint32_t deviceUptimeHour=((deviceUptime-(deviceUptimeMinute*60)-deviceUptimeSecond)%(60*60*24))/(60*60);
  //uint32_t deviceUptimeDay=((deviceUptime-(deviceUptimeHour*60*60)-(deviceUptimeMinute*60)-deviceUptimeSecond))/(60*60*24);
  
  String strDeviceUptimeSecond="";
  String strDeviceUptimeMinute="";
  String strDeviceUptimeHour="";


  if(deviceUptimeSecond<10)
  {
    strDeviceUptimeSecond="0"+String(deviceUptimeSecond);
  }
  else
  {
    strDeviceUptimeSecond=String(deviceUptimeSecond);
  }
  if(deviceUptimeMinute<10)
  {
    strDeviceUptimeMinute="0"+String(deviceUptimeMinute);
  }
  else
  {
    strDeviceUptimeMinute=String(deviceUptimeMinute);
  }
  if(deviceUptimeHour<10)
  {
    strDeviceUptimeHour="0"+String(deviceUptimeHour);
  }
  else
  {
    strDeviceUptimeHour=String(deviceUptimeHour);
  }

  //String strDeviceUptime=String(deviceUptimeDay)+"D-"+strDeviceUptimeHour+"H:"+strDeviceUptimeMinute+"M:"+strDeviceUptimeSecond+"S";
  String strDeviceUptime=strDeviceUptimeHour+"H:"+strDeviceUptimeMinute+"M:"+strDeviceUptimeSecond+"S";
  return strDeviceUptime;
}

void taskPrint()
{
  Serial.println(countDeviceUptime());
}

void taskBuzzer()
{
  for(int i=0;i<3;i++)
  {
    digitalWrite(BUZZER_PIN,HIGH);
    delay(2000);
    digitalWrite(BUZZER_PIN,LOW);
    delay(1000);
  }
}

void taskLCD()
{
  if((prevStatus!=status)||(prevCounter!=counter))
  {
    switch(status)
    {
      case SETUP_TIMER:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Setup Timer");
        lcd.setCursor(0,1);
        lcd.print("Yes<=");
        prevStatus=SETUP_TIMER;
        break;
      case INIT:
        prevCounter=counter;
        if(counter<=1)
          countStr=String(counter)+" minute";
        else
          countStr=String(counter)+" minutes";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(countStr.c_str());
        lcd.setCursor(0,1);
        lcd.print("Press SW_PIN");
        prevStatus=INIT;
        break;
      case INIT_YES:
        lcd.setCursor(0,1);
        lcd.print("Yes<=         No");
        prevStatus=INIT_YES;
        break;
      case INIT_NO:
        lcd.setCursor(0,1);
        lcd.print("Yes         =>No");
        prevStatus=INIT_NO;
        break;    
      case COUNTING:
        Serial.println("LCD COUNTING");
        prevCounter=counter;
        if(counter<=1)
          countStr=String(counter)+" minute";
        else
          countStr=String(counter)+" minutes";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(countStr.c_str());
        lcd.setCursor(0,1);
        lcd.print("Counting...");
        prevStatus=COUNTING;
        break;
      case IR_MODE:
        Serial.println("LCD IR_MODE");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("IR Receiver");
        prevStatus=IR_MODE;
        break;
      case IR_DATA_PAGE1:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Value:"+strValue);
        lcd.setCursor(0,1);
        lcd.print("Bits:"+strBits);
        prevStatus=IR_DATA_PAGE1;
        break;
      case IR_DATA_PAGE2:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Bits:"+strBits);
        lcd.setCursor(0,1);
        lcd.print("Decode Type:"+strDecodeType);
        prevStatus=IR_DATA_PAGE2;
        break;
      case DEVICE_UPTIME:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Device Uptime");
        lcd.setCursor(0,1);
        lcd.print(countDeviceUptime());
        //lcd.print("Not Supported");
        prevStatus=DEVICE_UPTIME;
        break;
      case LCD_SAVER:
        lcd.noBacklight();
        prevStatus=LCD_SAVER;
    }
  }
}

void taskCount()
{ 
  if(status==COUNTING)
  {
    millisNow=millis();
    if(millisEnd>millisNow)
    {
      counter=int(((millisEnd-millisNow)/1000)/60);
      if((millisEnd-millisNow)%60000!=0)
        counter=counter+1;
    }
    else
    {
      counter=0;
      prevCounter=counter;
      taskBuzzer();
      status=SETUP_TIMER;
    }
  }
}

void taskLCDSaver()
{
  millisLCDSaver=millis();
  if(millisLCDSaver-millisAction>60000&&status!=COUNTING)
  {
    /*if(status==IR_MODE||status==IR_DATA_PAGE1||status==IR_DATA_PAGE2)
    {
      irrecv.disableIRIn();
    }*/
    status=LCD_SAVER;
  }
}

void setup() 
{
  Serial.begin(115200);
  //irrecv.enableIRIn(); // 啟動接收
  
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(CLK_PIN,INPUT_PULLUP);
  pinMode(DT_PIN,INPUT_PULLUP);
  pinMode(SW_PIN,INPUT_PULLUP);
  digitalWrite(BUZZER_PIN,LOW);
  prev=digitalRead(CLK_PIN);

  Wire.begin();
  Wire.setClock(5000);
  
  lcd.init(); //初始化LCD 
  lcd.backlight(); //開啟背光
  lcd.setCursor(0,0);
  lcd.print("Setup Timer");
  lcd.setCursor(0,1);
  lcd.print("Yes<=");

  tickLCD.start();
  tickCount.start();
  tickLCDSaver.start();
  tickPrint.start();
}

void loop() 
{
  tickLCD.update();
  tickCount.update();
  tickLCDSaver.update();
  tickPrint.update();

  now=digitalRead(CLK_PIN);
  if(now!=prev)
  {
    if(digitalRead(DT_PIN)!=now)//順時鐘轉
    {
      millisAction=millis();
      switch(status)
      {
        case INIT:
          Serial.println("INIT++");
          plus++;
          if(plus==2)
          {
            counter++;
            plus=0;
          }
          if(counter>240)
            counter=240;
          break;
        case INIT_YES:
          Serial.println("INIT_YES=>INIT_NO");
          status=INIT_NO;
          break;
        case SETUP_TIMER:
          Serial.println("SETUP_TIMER=>IR_MODE");
          page++;
          if(page==2)
          {
            page=0;     
            irrecv.enableIRIn();  
            status=IR_MODE;
          }
          break;
        case IR_MODE:
          Serial.println("IR_MODE=>SETUP_TIMER");
          page++;
          if(page==2)
          {
            page=0;       
            irrecv.disableIRIn();   
            status=DEVICE_UPTIME;
          }          
          break;
        case DEVICE_UPTIME:
          Serial.println("IR_MODE=>DEVICE_UPTIME");
          page++;
          if(page==2)
          {
            page=0;       
            status=SETUP_TIMER;
          }          
          break;
        case IR_DATA_PAGE1:
          Serial.println("IR_DATA_PAGE1=>IR_DATA_PAGE2");
          status=IR_DATA_PAGE2;
          break;
        /*case IR_DATA_PAGE2:
          break;*/
        case LCD_SAVER:
          Serial.println("LCD_SAVER=>STUP_TIMER");
          lcd.backlight();
          status=SETUP_TIMER;
          break;
      }
    }
    else//逆時鐘轉
    {
      millisAction=millis();
      switch(status)
      {
        case INIT:
          Serial.println("INIT--");
          minus++;
          if(minus==2)
          {
            counter--;
            minus=0;
          }
          if(counter<0)
            counter=0;
          break;
        case INIT_NO:
          Serial.println("INIT_NO=>INIT_YES");
          status=INIT_YES;
          break;
        case SETUP_TIMER:
          Serial.println("SETUP_TIMER=>DEVICE_UPTIME");
          page++;
          if(page==2)
          {
            page=0;      
            status=DEVICE_UPTIME;
          }
          break;
        case IR_MODE:
          Serial.println("IR_MODE=>SETUP_TIMER");
          page++;
          if(page==2)
          {
            page=0;   
            irrecv.disableIRIn();       
            status=SETUP_TIMER;
          }
          break;
        case DEVICE_UPTIME:
          Serial.println("DEVICE_UPTIME=>IR_MODE");
          page++;
          if(page==2)
          {
            page=0;   
            irrecv.enableIRIn();       
            status=IR_MODE;
          }
          break;
        /*case IR_DATA_PAGE1:
          break;*/
        case IR_DATA_PAGE2:
          Serial.println("IR_DATA_PAGE2=>IR_DATA_PAGE1");
          status=IR_DATA_PAGE1;
          break;
        case LCD_SAVER:
          Serial.println("LCD_SAVER=>STUP_TIMER");
          lcd.backlight();
          status=SETUP_TIMER;
          break;
      }
    }
    //Serial.println(counter);
  }
  prev=now;

  if(digitalRead(SW_PIN)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(SW_PIN)==LOW){}
      millisAction=millis();
      switch(status)
      {
        case SETUP_TIMER:
          Serial.println("SETUP_TIMER=>INIT");
          status=INIT;
          break;
        case INIT:
          Serial.println("INIT=>INIT_YES");
          if(counter!=0)
            status=INIT_YES;
          break;
        case INIT_YES:
          Serial.println("INIT_YES=>COUNTING");
          status=COUNTING;
          millisNow=millis();
          millisEnd=millisNow+(counter*60000);
          break;
        case INIT_NO:
          Serial.println("INIT_NO=>SETUP_TIMER");
          status=SETUP_TIMER;
          counter=0;
          prevCounter=counter;
          break;
        case IR_DATA_PAGE1:
          Serial.println("IR_DATA_PAGE1=>IR_MODE");
          irrecv.resume();
          status=IR_MODE;
          break;
        case IR_DATA_PAGE2:
          Serial.println("IR_DATA_PAGE2=>IR_MODE");
          irrecv.resume();
          status=IR_MODE;
          break;
        case LCD_SAVER:
          Serial.println("LCD_SAVER=>STUP_TIMER");
          lcd.backlight();
          status=SETUP_TIMER;
          break;
      }
      Serial.println("SW_PIN");
      debounce=0;
    }
    debounce++;
  }

  if(status==IR_MODE&&irrecv.decode(&results))
  {
    Serial.println("IR_MODE=>IR_DATA_PAGE1");
    status=IR_DATA_PAGE1;
    strValue="";
    strValue=String(results.value,HEX);
    Serial.print("Value:"+strValue);// 輸出解碼後的資料

    strBits="";
    strBits=String(results.bits);
    Serial.print("Bits:"+strBits);
  
    strDecodeType="";
    strDecodeType=String(results.decode_type);
    Serial.println("Decode Type:"+strDecodeType);
   
     // 準備接收下一個訊號   
  }
} 
