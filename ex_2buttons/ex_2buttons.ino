
const int b1=5,b2=9;//,b3=13;//buttin pins
const int ledPin=6;
const int maxDebounce=500;
unsigned int debounce=0;

void setup() {//設定初始值
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  //pinMode(b3,INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);//初始化序列埠
  //啟用 Ethernet 連線，預設會以 DHCP 取得 IP 位址
  
}
void loop() 
{
  if(digitalRead(b1)==LOW)
  { 
    if(debounce>maxDebounce)
    {
      digitalWrite(ledPin,HIGH);
      while(digitalRead(b1)==LOW){
     
      }
      debounce=0;
      digitalWrite(ledPin,LOW);
      Serial.println("B1");
    }
    else
      debounce++; 
  }
  if(digitalRead(b2)==LOW)
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(b2)==LOW){
        digitalWrite(ledPin,HIGH);
        delay(500);
        digitalWrite(ledPin,LOW);
        delay(500);
    } 
      debounce=0;
      digitalWrite(ledPin,LOW);
      Serial.println("B3");   
    }
    else
      debounce++;
  }
  /*if(digitalRead(b3)==LOW)
  {
   while(digitalRead(b3)==LOW){} 
   Serial.println("B3");
   }*/
 
}
