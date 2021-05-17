
const int b1=11,b2=3;//,b3=13;//buttin pins
const int ledPin=13;

void setup() {//設定初始值
  pinMode(b1,INPUT);
  pinMode(b2,INPUT);
  //pinMode(b3,INPUT);
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);//初始化序列埠
  //啟用 Ethernet 連線，預設會以 DHCP 取得 IP 位址
  
}
void loop() 
{
  if(digitalRead(b1)==LOW)
  { 
    digitalWrite(ledPin,HIGH);
    while(digitalRead(b1)==HIGH){
         /*Serial.print("Port");
    Serial.print(11);
    Serial.print(": ");
    Serial.println(digitalRead(b2));*/
    }
    digitalWrite(ledPin,LOW);
    Serial.println("B1");
    
  }
  if(digitalRead(b2)==LOW)
  {
    while(digitalRead(b2)==HIGH){
      digitalWrite(ledPin,HIGH);
      delay(500);
      digitalWrite(ledPin,LOW);
      delay(500);
    } 
    Serial.println("B2");
    
  }
  /*if(digitalRead(b3)==LOW)
  {
   while(digitalRead(b3)==LOW){} 
   Serial.println("B3");
   }*/
  
} //無限迴圈
