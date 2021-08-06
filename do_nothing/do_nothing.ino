const byte potPin=A0;
const byte ledPin=2;
int val;

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin,OUTPUT);
}

void loop()
{
  //val=map(analogRead(potPin),0,1023,0,255);
  if(Serial.available() && (Serial.read()!=''))
  {
    val=Serial.parseInt();
    Serial.println(val);
    if(val>255)
      val=255;
    else if(val<0)
      val=0;
      
    
    //delay(1000);
  }
  analogWrite(ledPin,val);
}
