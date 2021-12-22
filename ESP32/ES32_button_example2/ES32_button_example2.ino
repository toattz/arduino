#define LED1 14
#define LED2 LED_BUILTIN
#define SW 13

bool isPressed=false;
bool isLongPressed=false;
bool LED1Status=false;
bool LED2Status=false;

typedef enum{
  RELEASED,
  PRESSED,
  LONG_PRESSED,
  PRESSING,
  RELEASED_FROM_PRESS,
  RELEASED_FROM_LONGPRESS
} SW_Status;

uint32_t pressTime=0;
uint8_t debounceTime=30;
uint16_t longPressTime=500;
uint32_t lastHoldTime=0;
uint8_t holdTime=200;

SW_Status checkSwitch(byte pin,bool ON=HIGH,bool pullup=false)
{
  SW_Status status=RELEASED;
  if(pullup)
  {
    pinMode(pin,INPUT_PULLUP);
  }
  else
  {
    pinMode(pin,INPUT);
  }

  if(digitalRead(pin)==ON)
  {
    if(!isPressed)
    {
      isPressed=true;
      status=PRESSED;
      pressTime=millis();      
    }
    if(isLongPressed&&(millis()-lastHoldTime>=holdTime))
    {
      status=PRESSING;
      Serial.println("Pressed");
      lastHoldTime=millis();      
    }
    if(!isLongPressed&&(millis()-pressTime>longPressTime))
    {
      isLongPressed=true;
      status=LONG_PRESSED;    
    }
  }
  else
  {
    if(isPressed)
    {
      if(isLongPressed==true)
      {
        status=RELEASED_FROM_LONGPRESS;
        isLongPressed=false;
        lastHoldTime=0;
      }
      else if(millis()-pressTime>debounceTime)
      {
        status=RELEASED_FROM_PRESS;
      }
      isPressed=false;
    }
  }
  return status;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(SW,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(checkSwitch(SW,LOW,true))
  {
    case RELEASED_FROM_PRESS:
      LED2Status=!LED2Status;
      digitalWrite(LED2,LED2Status);
      break;
    case RELEASED_FROM_LONGPRESS:
      LED1Status=!LED1Status;
      digitalWrite(LED1,LED1Status);
      break;
  }
}
