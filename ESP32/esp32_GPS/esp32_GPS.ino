void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial2.available()>0)
  {
    Serial.print(Serial2.read());
  }

}
