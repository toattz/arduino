#include<SD.h>
#include<WiFi.h>
#include<ESPAsyncWebServer.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
AsyncWebServer server(80);
uint32_t uploadCount=0;

void listFile()
{
  File root=SD.open("/");
  File file=root.openNextFile();
  while(file)
  {
    Serial.printf("Filename:%s size:%uMB\n",file.name(),file.size()/(1024*1024));
    file=root.openNextFile();
  }
  Serial.printf("SD card size:%uMB\n",SD.cardSize()/(1024*1024));
  Serial.printf("Total size:%uMB\n",SD.totalBytes()/(1024*1024));
  Serial.printf("Used size:%uMB\n",SD.usedBytes()/(1024*1024));
}

void handleUpload(AsyncWebServerRequest *req,String filename,size_t index,uint8_t *data,size_t len,bool final)
{
  if(!index)
  {
    Serial.printf("Start to upload:%s\n",filename.c_str());
    req->_tempFile=SD.open("/"+filename,"w");
  }

  if(len)
  {
    Serial.printf("upload count:%u\n",uploadCount);
    req->_tempFile.write(data,len);
    uploadCount+=1;
  }

  if(final)
  {
    Serial.printf("Uploaded:%s size%u\n",filename.c_str(),index+len);
    req->_tempFile.close();
    listFile();
    uploadCount=0;
  }
}

void setup()
{
  Serial.begin(115200);
  if(!SD.begin())
  {
    Serial.println("Unable to mount SD card");
    return;  
  }
  else
  {
    Serial.println("Mount SD card");
  }

  byte cardType=SD.cardType();

  switch(cardType)
  {
    case CARD_NONE:
      Serial.println("SD card is not inserted");
      return;
    case CARD_MMC:
      Serial.println("MMC");
      break;
    case CARD_SD:
      Serial.println("SDSC");
      break;
    case CARD_SDHC:
      Serial.println("SDHC");
      break;
    //case default:
      //Serial.println("未知");
  }

  WiFi.mode(WIFI_STA);
  WiFi.hostname("SD_UPLOAD");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());

  server.serveStatic("/",SD,"/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico",SD,"/www/favicon.ico");
  server.serveStatic("/firmware",SD,"/www/firmware.html");
  server.on("/upload",HTTP_POST,[](AsyncWebServerRequest *req){
    req->send(200,"text/html;charset=utf-8","<a href=/>回首頁</a>");
    },handleUpload);
  server.begin();
}

void loop() 
{


}
