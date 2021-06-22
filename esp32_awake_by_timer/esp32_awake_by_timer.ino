#define WAKEUP_MS 5*1000000

RTC_DATA_ATTR int wakes=0;

void setup() 
{
  Serial.begin(115200);
  Serial.printf("Wake up times:%d\n",wakes);
  wakes++;
  delay(1000);
  Serial.println("Zzz");
  delay(1000);
  esp_sleep_enable_timer_wakeup(WAKEUP_MS);
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:

}
