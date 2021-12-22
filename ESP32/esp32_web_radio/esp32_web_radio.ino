#include<Audio.h>

#define I2S_DOUT 25 //DIN
#define I2S_BCLK 27 //BCK
#define I2S_LRC 26 //LCK
#define MAX_VOL 21
#define VOL_UP 21
#define VOL_DOWN 18

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
byte volume=10;

Audio audio;

void setup() 
{
  pinMode(VOL_UP,INPUT_PULLUP);
  pinMode(VOL_DOWN,INPUT_PULLUP);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("Web_Radio");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  audio.setPinout(I2S_BCLK,I2S_LRC,I2S_DOUT);
  audio.setVolume(volume);
  //https://live.leanstream.co/ICRTFM-MP3?args=tunein_mp3
  //audio.connecttohost("https://live.leanstream.co/ICRTFM-MP3");
  //audio.connecttohost("http://tunein.streamguys1.com/TEDTalks");
  audio.connecttospeech("台北市副市長黃珊珊今天表示前立委丁守中在2月份已以第1類優先接種對象的身分造冊並上傳到全國性預防接種資訊管理系統","zh");
}

void loop() 
{
  audio.loop();

  if(digitalRead(VOL_DOWN)==LOW)
  {
    delay(20); 
    if(digitalRead(VOL_DOWN)==LOW)
    {
      if(volume>0)
      {
        volume--;
      }
      audio.setVolume(volume);
      Serial.println("volume down");
    }
  }

  if(digitalRead(VOL_UP)==LOW)
  {
    delay(20);
    if(digitalRead(VOL_UP)==LOW)
    {
      if(volume<MAX_VOL);
      {
        volume++;
      }
      audio.setVolume(volume);
      Serial.println("volume up");
    }
  }
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
