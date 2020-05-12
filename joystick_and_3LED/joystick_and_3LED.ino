int vrx,vry,sw;
char buf[100];
int LEDRpin=2;
int LEDGpin=3;
int LEDBpin=4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDRpin,OUTPUT);
  pinMode(LEDGpin,OUTPUT);
  pinMode(LEDBpin,OUTPUT);
  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  vrx=analogRead(A1);
  vry=analogRead(A2);
  sw=analogRead(A3);

  sprintf(buf,"VRx=%d, VRy=%d, SW=%d\n", vrx, vry, sw);
  Serial.print(buf);
  if(vrx <450)
  {
    digitalWrite(LEDRpin,HIGH);
  }
  else if(vrx >550)
  {
    digitalWrite(LEDRpin,LOW);   
  }
  else
  {
    digitalWrite(LEDBpin,HIGH);
  }

  if(vry < 450)
  {
    digitalWrite(LEDGpin,HIGH);
  }
  else if(vry >550)
  {
    digitalWrite(LEDGpin,LOW);
  }
  else
  {
    digitalWrite(LEDBpin,LOW);
  }
  delay(100);
}
