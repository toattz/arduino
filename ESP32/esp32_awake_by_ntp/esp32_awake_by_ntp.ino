#include<WiFi.h>
#include"sleepTimer.h"

const char * ssid="TOTOLINK N150RT";
const char * password="0422876333";
SleepTimer st(5);

void sendData()
{
  Serial.println("Send data");
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("wake up bt ntp");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  st.init(28800,0);
  int res=st.start(8,0,sendData);
  Serial.printf("res:%d\n",res);

}

void loop() {
  // put your main code here, to run repeatedly:

}
