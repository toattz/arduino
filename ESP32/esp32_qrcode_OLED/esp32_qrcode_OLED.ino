#include<WiFi.h>
#include<qrcode.h>
#include<ESPAsyncWebServer.h>
#include<U8g2lib.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";

AsyncWebServer server(80);
QRCode qrcode;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);

const char * homePage="<!doctype html>\
<html>\
  <head>\
    <meta charset='utf-8'>\
    <meta name='viewport' content='width=device-width,initial-scale=1>\
  </head>\
  <body>ESP32 website</body>\
</html>";

void drawQRCode(IPAddress ip)
{
  byte x0=3+64;
  byte y0=3;
  String url="http://"+ip.toString();
  uint8_t qrcodeData[qrcode_getBufferSize(3)];
  String subIP;
  qrcode_initText(&qrcode,qrcodeData,3,ECC_LOW,url.c_str());

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_VCR_OSD_tn);
    u8g2.setColorIndex(1);
    for(byte i=0;i<3;i++)
    {
      subIP=String(ip[i])+".";
      u8g2.drawUTF8(0,16*(i+1),subIP.c_str());
    }
    subIP=String(ip[3]);
    u8g2.drawUTF8(0,64,subIP.c_str());
    u8g2.drawBox(64,0,64,64);
    for(uint8_t y=0;y<qrcode.size;y++)
    {
      for(uint8_t x=0;x<qrcode.size;x++)
      {
        if(qrcode_getModule(&qrcode,x,y))
        {
          u8g2.setColorIndex(0);
        }
        else
        {
          u8g2.setColorIndex(1);
        }
        u8g2.drawBox(x0+x*2,y0+y*2,2,2);
      }
    }
  }while(u8g2.nextPage()); 
}

void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.hostname("QRCODE_OLED");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  server.on("/",HTTP_GET,[](AsyncWebServerRequest *req){
    req->send(200,"text/html",homePage);
    });
  server.begin();
  u8g2.begin();
  drawQRCode(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:

}
