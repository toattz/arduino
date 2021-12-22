int LEDRpin=2;
int LEDGpin=3;
int LEDBpin=4;
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDRpin,OUTPUT);
  pinMode(LEDGpin,OUTPUT);
  pinMode(LEDBpin,OUTPUT);
  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDRpin,HIGH);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,LOW);
  delay(1000);

  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,HIGH);
  digitalWrite(LEDBpin,LOW);
  delay(1000);

  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,HIGH);
  delay(1000);

}
