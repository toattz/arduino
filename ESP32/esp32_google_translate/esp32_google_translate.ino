#include<Audio.h>

#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

const char *texts[]={"讀書","睡覺","聽音樂","滑手機","外出走走"};
const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";
int8_t totalTexts=sizeof(texts)/sizeof(texts[0]);
int8_t index1=random(totalTexts);
String msg=String(texts[index1])+"的好天氣啊!";
Audio audio;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("google_translate");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  audio.setPinout(I2S_BCLK,I2S_LRC,I2S_DOUT);
  audio.setVolume(10);
  audio.connecttospeech(msg.c_str(),"zh");

}

void loop() {
  audio.loop();
}

// 事件處理函式
void audio_info(const char *info){
    printf("聲音資訊：%s\n", info);
}
void audio_showstation(const char *info){
    printf("電台   ：%s\n", info);
}
void audio_showstreaminfo(const char *info){
    printf("串流資訊：%s\n", info);
}
void audio_showstreamtitle(const char *info){
    printf("串流標題：%s\n", info);
}

void audio_eof_speech(const char * info)
{
  printf("%s 播放結束\n",info);
}
