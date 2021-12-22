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

#include<ArduinoJson.h>

#define CONFIG_FILE "/config.json"
#define DEBUG 1

char uuid[11];
char ssid[64];
char password[64];
char ver[10];

byte getInit()
{
  byte initStatus;
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getInit():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
      Serial.print(F("getInit() deserializeJson() failed: "));
      Serial.println(error.f_str());
      #endif
      
      initStatus=2;
    }  
    else
    {
      initStatus=readDoc["init"];
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getInit():No config.json");
    #endif
    initStatus=3;
  }
  file.close();
  return initStatus;  
}

void getVersion()
{
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getVersion():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getVersion() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
      strcpy(ver,"No version");
    }  
    else
    {
      strcpy(ver,readDoc["version"]);
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getVersion():No config.json");
    #endif      
    strcpy(ver,"No version");
  }
  file.close();
}

byte getOnboard()
{
  byte onboard;
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getOnboard():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getOnboard() deserializeJson() failed: "));
      #endif
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
    #if DEBUG
      Serial.println("getOnboard():No config.json");
    #endif
    onboard=3;
  }
  file.close();
  return onboard;  
}

void getUUID()
{
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getUUID():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getUUID() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
      strcpy(uuid,"No UUID");
    }  
    else
    {
      strcpy(uuid,readDoc["uuid"]);
      #if DEBUG
        Serial.printf("UUID:%s\n",uuid);
      #endif
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getUUID():No config.json");
    #endif
    strcpy(uuid,"No UUID");
  }
  file.close(); 
}

byte getConnected()
{
  byte connected;
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getConnected():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getConnected() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
      connected=2;
    }  
    else
    {
      connected=readDoc["connected"];
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getConnected():No config.json");
    #endif 
    connected=3;
  }
  file.close();
  return connected;    
}

void getSSID()
{
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getSSID():There is config.json");   
    #endif 
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getSSID() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
      strcpy(ssid,"No SSID");
    }  
    else
    {
      strcpy(ssid,readDoc["ssid"]);
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getSSID():No config.json");
    #endif
    strcpy(ssid,"No SSID");
  }
  file.close();   
}

void getPassword()
{
  File file=SPIFFS.open(CONFIG_FILE,"r");
  if(file&&file.size())
  {
    #if DEBUG
      Serial.println("getPassword():There is config.json");   
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc, file);
    if(error)
    {
      #if DEBUG
        Serial.print(F("getPassword() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
      strcpy(password,"");
    }  
    else
    {
      strcpy(password,readDoc["password"]);
    }
  }
  else
  {
    #if DEBUG
      Serial.println("getPassword():No config.json");
    #endif
    strcpy(password,"");
  }
  file.close();  
}

void setInit()
{
  #if DEBUG
    Serial.println("setUUID()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setUUID() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    { 
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setInit() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when initConfig"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setInit() no file&&file.size()");
    #endif
  }  
}

void setVersion(char *ver)
{
  #if DEBUG
    Serial.println("setVersion()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setVersion() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setVersion() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when set version"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setVersion() no file&&file.size()");
    #endif
  }  
}

void setOnboard(byte onboard)
{
  #if DEBUG
    Serial.println("setOnboard()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setOnboard() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setOnboard() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when set onboard"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setOnboard() no file&&file.size()");
    #endif
  }    
}

void setUUID() //for reset uuid, ex:disconnect from node.js server and generate new uuid
{
  #if DEBUG
    Serial.println("setUUID()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    Serial.println("setUUID() file&&file.size()");
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setInit() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
      StaticJsonDocument<512> writeDoc;
      writeDoc["init"]=readDoc["init"];
      writeDoc["version"]=readDoc["version"];
      writeDoc["onboard"]=readDoc["onboard"];
      strcpy(uuid,String(esp_random()).c_str()); //set uuid when init, 
      #if DEBUG
        Serial.printf("UUID:%s\n",uuid);
      #endif
      writeDoc["uuid"]=uuid;
      writeDoc["connected"]=readDoc["connected"];
      writeDoc["ssid"]=readDoc["ssid"];
      writeDoc["password"]=readDoc["password"];
      if(serializeJson(writeDoc,writeFile)==0)
      {
        #if DEBUG
          Serial.println(F("Failed to write to file when set uuid"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setUUID() no file&&file.size()");
    #endif
  }  
}

void setConnected(byte connection)
{
  #if DEBUG
    Serial.println("setConnected()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setConnected() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setConnected() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when set connected"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setConnected() no file&&file.size()");
    #endif
  }      
}

void setSSID()
{
  #if DEBUG
    Serial.println("setSSID()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setSSID() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setSSID() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when set ssid"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setSSID() no file&&file.size()");
    #endif
  }        
}

void setPassword()
{
  #if DEBUG
    Serial.println("setPassword()");
  #endif
  File readFile=SPIFFS.open(CONFIG_FILE,"r");
  if(readFile&&readFile.size())
  { 
    #if DEBUG
      Serial.println("setPassword() file&&file.size()");
    #endif
    StaticJsonDocument<512> readDoc;
    DeserializationError error = deserializeJson(readDoc,readFile);
    readFile.close();
    if (error)
    {
      #if DEBUG
        Serial.println("error");
        Serial.print(F("setPassword() deserializeJson() failed: "));
        Serial.println(error.f_str());
      #endif
    }
    else
    {
      File writeFile=SPIFFS.open(CONFIG_FILE,"w");
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
        #if DEBUG
          Serial.println(F("Failed to write to file when set password"));
        #endif
      }
      writeFile.close();
    }
  }
  else
  {
    #if DEBUG
      Serial.println("setPassword() no file&&file.size()");
    #endif
  }          
}

void getJson()
{
  Serial.printf("init:%d\nonboard:%d\nconnected:%d\n",getInit(),getOnboard(),getConnected());
}
