int vrx,vry,sw;
char buf[100];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  vrx=analogRead(A1);
  vry=analogRead(A2);
  sw=analogRead(A3);

  sprintf(buf,"VRx=%d, VRy=%d, SW=%d\n", vrx, vry, sw);
  Serial.print(buf);
  delay(100);
}
