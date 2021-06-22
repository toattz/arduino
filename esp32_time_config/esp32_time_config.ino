#include<WiFi.h>
#include<time.h>

const char * ssid="TOTOLINK N150RT";
const char * password="0422876333";
const char * ntpServer="pool.ntp.org";
const uint16_t utcOffset=28800;
const uint8_t daylightOffset=0;

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("test auot config time");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  configTime(utcOffset,daylightOffset,ntpServer);  
  delay(1000);
}

void loop() 
{
  struct tm now;
  if(!getLocalTime(&now))
  {
    Serial.println("can not get local time");
    return;
  }
  Serial.println(&now,"%Y/%m/%d %H:%M:%S");
  delay(1000);

}
