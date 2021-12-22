#include<WiFi.h>
#include<ESPAsyncWebServer.h>
#include<SPIFFS.h>

const char * ssid="ZYXEL_1F";
const char * password="C1470A9A";
AsyncWebServer server(80);

void listFile()
{
  File root=SPIFFS.open("/");
  File file=root.openNextFile();
  while(file)
  {
    Serial.printf("file name:%s\n",file.name());
    file=root.openNextFile();
  }
}

void handleUpload(AsyncWebServerRequest *req,String filename,size_t index,uint8_t *data,size_t len,bool final)
{
  if(!index)
  {
    Serial.printf("Start upload:%s\n",filename.c_str());
    req->_tempFile=SPIFFS.open("/www/data/"+filename,"w");
  }

  if(len)
  {
    req->_tempFile.write(data,len);
  }

  if(final)
  {
    Serial.printf("Uploaded:%s size:%u\n",filename.c_str(),index,len);
    req->_tempFile.close();
    listFile();
  }
}

void setup() 
{
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    while(1)
    {
      delay(50);
    }
  }
  WiFi.mode(WIFI_STA);
  WiFi.hostname("UPLOAD_FILE");
  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.printf("IP address:%s\n",WiFi.localIP().toString().c_str());

  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/favicon.ico",SPIFFS,"/www/favicon.ico");
  server.serveStatic("/firmware",SPIFFS,"/www/firmware.html");
  server.on("/upload",HTTP_POST,[](AsyncWebServerRequest *req){
    req->send(200,"text/html;charset=utf-8","<a href=/>回首頁</a>");  
  },handleUpload);
  server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}
