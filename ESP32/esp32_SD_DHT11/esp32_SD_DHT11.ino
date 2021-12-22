#include<WiFi.h>
#include<SD.h>
#include<time.h>
#include<DHT.h>

#define DHTPIN 16
#define DHTTYPE DHT11
#define WAKE_TIME 60*1000000L

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
const char * ntpServer="pool.ntp.org";
const uint16_t utcOffset=28800;
const uint8_t daylightOffset=0;

DHT dht(DHTPIN,DHTTYPE);

void saveData()
{
  float t=dht.readTemperature();
  float h=dht.readHumidity();

    time_t now=time(NULL);
    struct tm *localtm=localtime(&now);
    Serial.println(localtm,"%Y/%m/%d %H:%M:%S");
    Serial.printf("Temperature:%1f Humidity:%1f\n",t,h);

    File file=SD.open("/log.txt","a");
    if(!file)
    {
      Serial.println("Open file fail");
      return;
    }
    file.print(localtm,"%Y/%m/%d %H:%M:%S");
    file.printf(",%1f,%1f\n",t,h);
    file.close();
}

void setup() 
{
  Serial.begin(115200);

  if(!SD.begin())
  {
    Serial.println("Mount SD fail");
    return;
  }
  else
  {
    Serial.println("Mount SD success");
  }

  byte cardType=SD.cardType();
  switch(cardType)
  {
    case CARD_NONE:
      Serial.println("SD card is not iserted");
      break;
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SDSC");
      break;
    case CARD_SDHC:
      Serial.println("SDHC");
      break;     
  }

  dht.begin();
  
  WiFi.mode(WIFI_STA);
  WiFi.hostname("SD_DHT11");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  configTime(utcOffset,daylightOffset,ntpServer);
  delay(2000);
  saveData();
  delay(100);
  esp_sleep_enable_timer_wakeup(WAKE_TIME);
  Serial.println("ZZZ");
  esp_deep_sleep_start(); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
