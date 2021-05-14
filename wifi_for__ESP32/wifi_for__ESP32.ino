#include <WiFi.h>

const char *ssid = "TP-LINK_470A9A_EXT"; //你的网络名称
const char *password = "C1470A9A"; //你的网络密码

void setup()
{
  Serial.begin(115200);
  Serial.println();
  delay(10000);

  WiFi.begin(ssid, password); //连接网络

  while (WiFi.status() != WL_CONNECTED) //等待网络连接成功
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); //打印模块IP
}

void loop()
{
}
