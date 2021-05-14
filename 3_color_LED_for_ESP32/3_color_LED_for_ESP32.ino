int LEDRpin=18;
int LEDGpin=19;
int LEDBpin=21;
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDRpin,OUTPUT);
  pinMode(LEDGpin,OUTPUT);
  pinMode(LEDBpin,OUTPUT);
  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDRpin,HIGH);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,LOW);
  Serial.write("LEDRpin\n"); 
  delay(1000);

  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,HIGH);
  digitalWrite(LEDBpin,LOW);
  Serial.write("LEDGpin\n"); 
  delay(1000);

  digitalWrite(LEDRpin,LOW);
  digitalWrite(LEDGpin,LOW);
  digitalWrite(LEDBpin,HIGH);
  Serial.write("LEDBpin\n"); 
  delay(1000);

}
