
#define CLK_PIN 17 
#define DT_PIN 25
#define SW_PIN 26

bool prev=LOW;
bool now=LOW;
uint16_t maxDebounce=500;
uint16_t debounce=0;

void setup()
{
  Serial.begin(115200);
  pinMode(CLK_PIN,INPUT_PULLUP);
  pinMode(DT_PIN,INPUT_PULLUP);
  pinMode(SW_PIN,INPUT_PULLUP);
  prev=digitalRead(CLK_PIN);
}

void loop()
{
  now=digitalRead(CLK_PIN);
  if(now!=prev)
  {
    if(digitalRead(DT_PIN)!=now)
    {
      Serial.println("turn right");
    }
    else
    {
      Serial.println("turn left");
    }
  }
  prev=now;

  if(digitalRead(SW_PIN)==LOW)
  {
    Serial.println("digitalRead(SW_PIN)==LOW");
    if(debounce<maxDebounce)
    {
      debounce++;
    }
    else
    {
      while(digitalRead(SW_PIN)==LOW){}
      Serial.println("Press SW");
      
      debounce=0;
    }
  }
}
