#include<SPIFFS.h>

void writeFile()
{
  File file=SPIFFS.open("/test.txt","w");
  if(!file)
  {
    Serial.println("Can not open file");
    return ;
  }
  file.println("write data");
  file.close();
}

void readFile()
{
  File file=SPIFFS.open("/test.txt","r");
  if(!file)
  {
    Serial.println("Can not open file");
    return ;
  }
  Serial.printf("File size:%u\n",file.size());
  Serial.println("File content:");
  while(file.available())
  {
    Serial.write(file.read());
  }
  file.close();
  
}

void listFile()
{
  File root=SPIFFS.open("/");
  File file=root.openNextFile();
  while(file)
  {
    Serial.printf("File name:%s\n",file.name());
    file=root.openNextFile();
  }
  Serial.printf("Total Size:%u\n",SPIFFS.totalBytes());
  Serial.printf("Used Size:%u\n",SPIFFS.usedBytes());
}

void setup() 
{
  Serial.begin(115200);

  if(!SPIFFS.begin(true))
  {
    Serial.println("Unable to mount SPIFFS");
    while(1)
      delay(10);
  }

  writeFile();
  readFile();
  listFile();

}

void loop()
{
  // put your main code here, to run repeatedly:

}
