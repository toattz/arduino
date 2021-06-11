#include<HTTPClient.h>
#include<ArduinoJson.h>
#include<U8g2lib.h>

HTTPClient http;
String key="939f3c95307da3e76d4fa16c70b81b93";
String cities[]={"taipei","taichung","hsinchu","kaohsiung","tainan","pingtung","taoyuan","maoli"};
char * ssid="ZYXEL_1F";
char * password="C1470A9A";
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);

void parseWeather(String json)
{
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, json);
  JsonObject weather = doc["weather"][0];
  const char* weather_icon = weather["icon"]; 
  JsonObject main = doc["main"];
  float main_temp = (float)main["temp"]-273.15; 
  int main_humidity = (int)main["humidity"]; 
  const char* city = doc["name"];
  
  Serial.printf("Weather icon:%s\n",weather_icon);
  Serial.printf("Temp:%.1f\n",main_temp);
  Serial.printf("Humidity:%d\n",main_humidity);

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_profont12_mr);
    u8g2.drawUTF8(0,8,city);
    u8g2.setFont(u8g2_font_inr16_mf);
    u8g2.setCursor(60,36);
    u8g2.print(String(main_temp,1)+"\xb0");
    u8g2.setCursor(60,62);
    u8g2.print(String(main_humidity)+"%");
    //u8g2.sendBuffer();
  }while (u8g2.nextPage());
}


String openWeather(String city)
{
  String url="http://api.openweathermap.org/data/2.5/weather?q="+city+"&appid="+key;
  http.begin(url);
  int httpCode=http.GET();
  String payload;
  if(httpCode>0)
  {
    payload=http.getString();
    Serial.println(payload.c_str());
  }
  else
  {
    Serial.println("Request is error");
  }
  http.end();
  return payload;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("Get_Weather");
  WiFi.begin(ssid,password);

  while (WiFi.status()!=WL_CONNECTED)
    delay(500);

  Serial.printf("IP address:%s\n",WiFi.localIP().toString().c_str());
  //Serial.printf("cities size:%d\n",sizeof(cities));

  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<8;i++)
  {
    String payload=openWeather(cities[i]);

    if(payload!="")
    {
      parseWeather(payload);
    }
    delay(10000);
  }
}
