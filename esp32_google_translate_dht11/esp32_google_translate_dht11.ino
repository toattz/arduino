#include<Audio.h>
#include<DHT.h>

#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26
#define DHTTYPE DHT11
#define DHTPIN 18
#define SW 21

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
char *texts[]={"讀書","睡覺","聽音樂","滑手機","外出走走"};
DHT dht(DHTPIN,DHTTYPE);
Audio audio;
const uint8_t debounceDelay=20;
bool activate=false;
uint8_t totalTexts=sizeof(texts)/sizeof(texts[0]);



void setup() 
{
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  pinMode(SW,INPUT_PULLUP);

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("DHT11_PCM5102");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  audio.setPinout(I2S_BCLK,I2S_LRC,I2S_DOUT);
  audio.setVolume(12);
}

void loop()
{
  bool swState=digitalRead(SW);
  String msg;

  if(!activate&&swState==LOW)
  {
    delay(debounceDelay);
    if(swState==digitalRead(SW))
    {
      activate=true;
      digitalWrite(LED_BUILTIN,LOW);
      uint16_t t=dht.readTemperature();
      if(isnan(t))
      {
        msg="無法取得溫度";
      }
      else
      {
        uint8_t index1=random(totalTexts);
        msg="現在溫度"+String(t)+"度真是"+texts[index1]+"的好天氣啊";
      }
      audio.connecttospeech(msg.c_str(),"zh");
    }
  }
  audio.loop();
}

void audio_eof_speech(const char * info)
{
  digitalWrite(LED_BUILTIN,HIGH);
  activate=false;
  Serial.printf("%s播放結束\n",info);
}
