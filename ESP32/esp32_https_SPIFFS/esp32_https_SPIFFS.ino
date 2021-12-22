#include<WiFiClientSecure.h>
#include<SPIFFS.h>

#define CA_FILE "/thingspeak.cer"
#define CHANNEL_ID "1404022"
#define API_KEY "PKCUW8S5UH9FHIVD"

const char *ssid="ZYXEL_1F";
const char *password="C1470A9A";
const char *SERVER="api.thingspeak.com";
String root_ca="44";
WiFiClientSecure client;

bool readFile(String path)
{
  File file=SPIFFS.open(path,"r");
  if(!file)
  {
    Serial.println("Open file fail");
    return 0;
  }
  /*else
  {
    root_ca=file.readString();
    file.close();
    return 1;
  }*/
  Serial.printf("Read file before:%s\n",root_ca.c_str());
  root_ca=file.readString();
  Serial.printf("Read file after:%s\n",root_ca.c_str());
  file.close();
  return 1;
}

void setup() 
{
  Serial.begin(115200);
  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.hostname("HTTPS_SPIFFS");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
  
  if(readFile(CA_FILE))
  {
    Serial.println("client.setCACert");
    Serial.print(root_ca.c_str());
    client.setCACert(root_ca.c_str());
  }
  else
  {
    Serial.println("client.setCACert fail");
    while(1)
    {
      delay(10);
    }
  }
  Serial.println("Start to connect server");

  if(!client.connect(SERVER,443))
  {
    Serial.println("Connection fail");
    return;
  }
  else
  {
    Serial.println("Connection success");
    String httpGet = "GET https://" + String(SERVER) + "/channels/" + CHANNEL_ID + "/feeds.json?api_key=" + API_KEY + "&results=2 HTTP/1.1\nHost:" + String(SERVER) + "\nConnection:close\n\n";
    client.print(httpGet);
    
    while(client.connected())
    {
      String line=client.readStringUntil('\n');
      if(line=="\r")
      {
        Serial.print("Get response");
        break;
      }
    }

    while(client.available())
    {
      char c=client.read();
      Serial.write(c);
    }
    client.stop();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
