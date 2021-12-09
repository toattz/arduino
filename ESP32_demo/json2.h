/*  config.json
{
  "init":1,
  "version":"0.1",
  "onboard":0,
  "uuid":"2860891901",
  "connected":1,
  "ssid":"ZYXEL_1F",
  "password":"C1470A9A"
}
*/
#include<SD.h>
#include<ArduinoJson.h>

#define CONFIG_FILE "/config.json"

char uuid[11];
char ssid[64];
char password[64];
char ver[10];

byte getInit()
{
  byte initStatus;
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getInit():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getInit() deserializeJson() failed: "));
      Serial.println(error.f_str());
      initStatus=2;
    }  
    else
    {
      initStatus=readDoc["init"];
    }
  }
  else
  {
    Serial.println("getInit():No config.json");
    initStatus=3;
  }
  file.close();
  return initStatus;  
}

void getVersion()
{
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getVersion():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getVersion() deserializeJson() failed: "));
      Serial.println(error.f_str());
      strcpy(ver,"No version");
    }  
    else
    {
      strcpy(ver,readDoc["version"]);
    }
  }
  else
  {
    Serial.println("getVersion():No config.json");
    strcpy(ver,"No version");
  }
  file.close();
}

byte getOnboard()
{
  byte onboard;
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getOnboard():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getOnboard() deserializeJson() failed: "));
      Serial.println(error.f_str());
      onboard=2;
    }  
    else
    {
      onboard=readDoc["onboard"];
    }
  }
  else
  {
    Serial.println("getOnboard():No config.json");
    onboard=3;
  }
  file.close();
  return onboard;  
}

void getUUID()
{
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getUUID():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getUUID() deserializeJson() failed: "));
      Serial.println(error.f_str());
      strcpy(uuid,"No UUID");
    }  
    else
    {
      strcpy(uuid,readDoc["uuid"]);
      Serial.printf("UUID:%s\n",uuid);
    }
  }
  else
  {
    Serial.println("getUUID():No config.json");
    strcpy(uuid,"No UUID");
  }
  file.close(); 
}

byte getConnected()
{
  byte connected;
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getConnected():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getConnected() deserializeJson() failed: "));
      Serial.println(error.f_str());
      connected=2;
    }  
    else
    {
      connected=readDoc["connected"];
    }
  }
  else
  {
    Serial.println("getConnected():No config.json");
    connected=3;
  }
  file.close();
  return connected;    
}

void getSSID()
{
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getSSID():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getSSID() deserializeJson() failed: "));
      Serial.println(error.f_str());
      strcpy(ssid,"No SSID");
    }  
    else
    {
      strcpy(ssid,readDoc["ssid"]);
    }
  }
  else
  {
    Serial.println("getSSID():No config.json");
    strcpy(ssid,"No SSID");
  }
  file.close();   
}

void getPassword()
{
  File file=SD.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    Serial.println("getPassword():There is config.json");   
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      Serial.print(F("getPassword() deserializeJson() failed: "));
      Serial.println(error.f_str());
      strcpy(password,"");
    }  
    else
    {
      strcpy(password,readDoc["password"]);
    }
  }
  else
  {
    Serial.println("getPassword():No config.json");
    strcpy(password,"");
  }
  file.close();  
}

void setInit()
{
  Serial.println("setUUID()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setUUID() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setInit() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=1;
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      strcpy(uuid,String(esp_random()).c_str()); //set uuid when init, 
      Serial.printf("UUID:%s\n",uuid);
      writeDoc["uuid"]=uuid;
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when initConfig"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setInit() no file&&file.size()");
  }  
}

void setVersion(char *ver)
{
  Serial.println("setVersion()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setVersion() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setVersion() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=ver;
      writeDoc["onboard"]=readDoc["onboard"];
      writeDoc["uuid"]=readDoc["uuid"];
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set version"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setVersion() no file&&file.size()");
  }  
}

void setOnboard(byte onboard)
{
  Serial.println("setOnboard()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setOnboard() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setOnboard() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=onboard;
      writeDoc["uuid"]=readDoc["uuid"];
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set onboard"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setOnboard() no file&&file.size()");
  }    
}

void setUUID() //for reset uuid, ex:disconnect from node.js server and generate new uuid
{
  Serial.println("setUUID()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setUUID() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setInit() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      strcpy(uuid,String(esp_random()).c_str()); //set uuid when init, 
      Serial.printf("UUID:%s\n",uuid);
      writeDoc["uuid"]=uuid;
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set uuid"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setUUID() no file&&file.size()");
  }  
}

void setConnected(byte connection)
{
  Serial.println("setConnected()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setConnected() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setConnected() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      writeDoc["uuid"]=readDoc["uuid"];
      writeDoc["connected"]=connection;
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set connected"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setConnected() no file&&file.size()");
  }      
}

void setSSID()
{
  Serial.println("setSSID()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setSSID() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setSSID() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      writeDoc["uuid"]=readDoc["uuid"];
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=ssid;
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set ssid"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setSSID() no file&&file.size()");
  }        
}

void setPassword()
{
  Serial.println("setPassword()");
  File readFile=SD.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setPassword() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      Serial.println("error");
      Serial.print(F("setPassword() deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    else
    {
      File writeFile=SD.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      writeDoc["uuid"]=readDoc["uuid"];
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=password;
      if(serializeJson(writeDoc,writeFile)==0)
      {
        Serial.println(F("Failed to write to file when set password"));
      }
      writeFile.close();
    }
  }
  else
  {
    Serial.println("setPassword() no file&&file.size()");
  }          
}

void getJson()
{
  Serial.printf("init:%d\nonboard:%d\nconnected:%d\n",getInit(),getOnboard(),getConnected());
}
