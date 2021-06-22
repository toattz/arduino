#define THRESHOLD 40
const byte touchPin=T3;
RTC_DATA_ATTR int wakes=0;

void callback()
{}

void setup()
{
  Serial.begin(115200);
  Serial.printf("Wake up times:%d\n",wakes);
  wakes++;
  delay(1000);
  Serial.println("zzz");
  delay(1000);
  touchAttachInterrupt(touchPin,callback,THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  esp_deep_sleep_start();

}

void loop() 
{


}
