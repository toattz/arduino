#include<LiquidCrystal_I2C.h> 
#include<Wire.h> 
#include<Ticker.h>

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

int counter=0;
int prevCounter=0;
int plus=0;
int minus=0;
const int maxDebounce=500;
uint32_t millisEnd=0;
uint32_t millisNow=0;
uint32_t millisRemain=0;
uint32_t debounce=0;
bool now=0;
bool prev=0;
String countStr="";

void taskLCD();
void taskCount();

LiquidCrystal_I2C lcd(0x27,16,2);  //設定LCD位置0x27,設定LCD大小為16*2
Ticker tickLCD(taskLCD,200);
Ticker tickCount(taskCount,1000);
typedef enum{
  NONE,
  IDLE,
  INIT,
  INIT_YES,
  INIT_NO,
  SETUP,
  COUNTING
}SystemStatus;
SystemStatus status=IDLE;
SystemStatus prevStatus=IDLE;

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
      case IDLE:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Setup Timer");
        lcd.setCursor(0,1);
        lcd.print("Yes<=");
        prevStatus=IDLE;
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
      status=IDLE;
    }
  }
}

void setup() 
{
  Serial.begin(115200);

  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(CLK_PIN,INPUT_PULLUP);
  pinMode(DT_PIN,INPUT_PULLUP);
  pinMode(SW_PIN,INPUT_PULLUP);
  digitalWrite(BUZZER_PIN,LOW);
  prev=digitalRead(CLK_PIN);
  
  lcd.init(); //初始化LCD 
  lcd.backlight(); //開啟背光
  lcd.setCursor(0,0);
  lcd.print("Setup Timer");
  lcd.setCursor(0,1);
  lcd.print("Yes<=");

  tickLCD.start();
  tickCount.start();
}

void loop() 
{
  tickLCD.update();
  tickCount.update();

  now=digitalRead(CLK_PIN);
  if(now!=prev)
  {
    if(digitalRead(DT_PIN)!=now)
    {
      switch(status)
      {
        case INIT:
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
          status=INIT_NO;
          break;
      }
    }
    else
    {
      switch(status)
      {
        case INIT:
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
          status=INIT_YES;
          break;
      }
    }
    Serial.println(counter);
  }
  prev=now;

  if(digitalRead(SW_PIN)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(SW_PIN)==LOW){}
      switch(status)
      {
        case IDLE:
          status=INIT;
          break;
        case INIT:
          if(counter!=0)
            status=INIT_YES;
          break;
        case INIT_YES:
          status=COUNTING;
          millisNow=millis();
          millisEnd=millisNow+(counter*60000);
          break;
        case INIT_NO:
          status=IDLE;
          counter=0;
          prevCounter=counter;
          break;
      }
      Serial.println("SW_PIN");
      debounce=0;
    }
    debounce++;
  }
} 
