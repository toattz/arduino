const byte LED1=14;
const byte LED2=LED_BUILTIN;
const byte SW=13;

bool LED1State=false;
bool LED2State=false;

uint32_t pressTime=0;
uint8_t debounceTime=30;
uint16_t longPressTime=500;
uint32_t lastHoldTime=0;
uint8_t holdTime=200;

bool isPress=false;
bool isLongPress=false;

void checkSwitch()
{
  if(digitalRead(SW)==LOW)
  {
    if(!isPress)
    {
      Serial.println("if(!isPress)");
      isPress=true;
      pressTime=millis();
    }

    if(isLongPress&&(millis()-lastHoldTime>=holdTime))
    {
      Serial.println("if(isLongPress&&(millis()-lastHoldTime>=holdTime))");
      Serial.println("Pressing");
      lastHoldTime=millis();
    }
    
    if(!isLongPress&&(millis()-pressTime>longPressTime))
    { 
      Serial.println("if(!isLongPress&&(millis()-pressTime>longPressTime))");
      Serial.printf("LED1:%d\n",LED1State);
      isLongPress=true;
      LED1State=!LED1State;
      digitalWrite(LED1,LED1State);      
    }
  }
  else
  {
    if(isPress)
    {
      Serial.println("if(isPress)");
      if(isLongPress)
      {
        Serial.println("if(isLongPress)");
        isLongPress=false;
        lastHoldTime=0;
      }
      else if((millis()-pressTime)>debounceTime)
      {
        Serial.println("else if((millis()-pressTime)>debounceTime)");
        Serial.printf("LED2:%d\n",LED2State);
        LED2State=!LED2State;
        digitalWrite(LED2,LED2State);
      }
      Serial.println("isPress=false");
      isPress=false;
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(SW,INPUT_PULLUP);
}

void loop()
{
  checkSwitch();
}
