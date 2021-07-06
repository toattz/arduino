const byte CLK_PIN=19;
const byte DT_PIN=21;
int counter=0;
bool now=0;
bool prev=0;

void setup() 
{
  Serial.begin(115200);
  pinMode(CLK_PIN,INPUT_PULLUP);
  pinMode(DT_PIN,INPUT_PULLUP);

  prev=digitalRead(CLK_PIN);
}

void loop() 
{
  now=digitalRead(CLK_PIN);
  //Serial.printf("now=%d\n",now);
  if(now!=prev)
  {
    if(digitalRead(DT_PIN)!=now)
    {
      counter++;
    }
    else
    {
      counter--;
    }
    Serial.printf("Counter:%d\n",counter);
  }
  prev=now;
}
