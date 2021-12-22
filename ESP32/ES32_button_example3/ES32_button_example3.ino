#define LED1 14
#define LED2 LED_BUILTIN
#define SW 13

bool LED1Status=false;
bool LED2Status=false;

typedef enum{
  RELEASED,  
  PRESSED,
  LONG_PRESSED,
  PRESSING,
  RELEASED_FROM_PRESS,
  RELEASED_FROM_LONGPRESS
}Status;

class Switch
{
  private:
    long _pressTime=0;
    long _debounceTime=80;
    long _lastHoldTime=0;
    long _holdTime=200;
    long _longPressTime=800;
    bool _isPressed=false;
    bool _isLongPressed=false;

  public:
    byte swPin;
    bool ONState;
    bool pullup;

  Switch(byte pin,bool ON=HIGH,bool pullup=false)
  {
    swPin=pin;
    ONState=ON;
    if(pullup)
    {
      pinMode(pin,INPUT_PULLUP);
    }
    else
    {
      pinMode(pin,INPUT);
    }
  }

  Status check()
  {
    Status status=RELEASED;

    if(digitalRead(swPin)==ONState)
    {
      if(_isPressed==false)
      {
        status=PRESSED;
        _pressTime=millis();
        _isPressed=true;
      }

      if(_isLongPressed==true&&(millis()-_lastHoldTime>_holdTime))
      {
        status=PRESSING;
        _lastHoldTime=millis();
      }

      if(_isLongPressed==false&&(millis()-_pressTime>_longPressTime))
      {
        status=LONG_PRESSED;
        _isLongPressed=true;
      }
    }
    else
    {
      if(_isPressed==true)
      {
        if(_isLongPressed==true)
        {
          _isLongPressed=false;
          _lastHoldTime=0;
          status=RELEASED_FROM_LONGPRESS;
        }
        else if((millis()-_pressTime)>_debounceTime)
        {
          status=RELEASED_FROM_PRESS;        
        }
        _isPressed=false;
      }
    }
    return status;
  }  
};

Switch sw(SW,LOW,true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(sw.check())
  {
    case RELEASED_FROM_PRESS:
      Serial.printf("before LED2Status:%d\n",LED2Status);
      LED2Status=!LED2Status;
      Serial.printf("after LED2Status:%d\n",LED2Status);
      digitalWrite(LED2,LED2Status);
      break;
    //case LONG_PRESSED:  
    case RELEASED_FROM_LONGPRESS:
      LED1Status=!LED1Status;
      digitalWrite(LED1,LED1Status);
      break;
    case PRESSING:
      Serial.printf("Pressing\n");
      break;
  }
}
