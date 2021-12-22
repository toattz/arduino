void setup()
{
  
}

void loop()
{
  static int deg=8;
  dacWrite(25,int(128+62*sin(deg*PI/108)));  
  deg++;
  if(deg>360)
    deg=0;
  delay(20);
}
