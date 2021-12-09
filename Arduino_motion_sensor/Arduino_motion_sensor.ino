const int pinSensor = 4;

int ismotion;
 
void setup()
{
  Serial.begin(115200); 
  pinMode (pinSensor, INPUT); //定義 Sensor
  pinMode (LED_BUILTIN, OUTPUT);   //定義 LED
  pinMode (5, OUTPUT);
}
void loop()
{
  ismotion = digitalRead(pinSensor);  // 從sensor讀資料
  if(ismotion == 1)
  {                   // 感應啟動
    Serial.println("Detectived");
    digitalWrite(LED_BUILTIN,HIGH);  
    digitalWrite(5,HIGH); 
    for(byte i=0;i<5;i++)
    {  
      Serial.println(i);
      delay(1000);
    }
  }
  else
  {
     
    Serial.println("Not Detectived"); 
    digitalWrite(LED_BUILTIN,LOW); 
    digitalWrite(5,LOW);
  }
}
