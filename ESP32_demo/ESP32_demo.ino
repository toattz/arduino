#include"demo.h"
void setup()
{
  Serial.begin(115200);
  delay(2000);
  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS is broken");
    return;
  }
  checkConfig(); //to check UUID and initialized
  if(status==WIFI_NOT_SCANNED)
  {
    scanWIFI();
  }
   
  BT.setPin(pin);
  BT.begin(String("ESP32 ")+uuid);

  byte macBT[6];
  esp_read_mac(macBT,ESP_MAC_BT);
  Serial.printf("BT MAC Address:%02X:%02X:%02X:%02X:%02X:%02X\n",macBT[0],macBT[1],macBT[2],macBT[3],macBT[4],macBT[5]);

  byte connectedStatus=getConnected();
  if(connectedStatus==1)
  {
    getSSID();
    getPassword();
    Serial.printf("connectedStatus==1, ssid:%s, password:%s\n",ssid,password);
    WiFi.begin(ssid,password);
    while(WiFi.status()!=WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
    Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
    status=WIFI_CONNECTED;
  }
  else
  {
    Serial.printf("connectedStatus==0\n");
  }
  
  server.serveStatic("/",SPIFFS,"/www/").setDefaultFile("index.html");
  server.serveStatic("/img",SPIFFS,"/www/img/");
  server.serveStatic("/favicon",SPIFFS,"/www/favicon.ico");
  server.on("/info",HTTP_GET,[](AsyncWebServerRequest * req){
    DynamicJsonDocument json(256);
    json["status"] = st[status];
    json["ssid"] = WiFi.SSID();
    json["ip"] = WiFi.localIP().toString();
    json["uuid"]=uuid;
    String res;
    serializeJson(json, res);
    req->send(200,"application/json",res);
    });
  server.on("/wifiList",HTTP_GET,[](AsyncWebServerRequest * req){
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, ssidList);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    DynamicJsonDocument json(512);
    for (JsonObject device : doc["devices"].as<JsonArray>())
    {
      int device_id = device["id"]; // 1, 2, 3, 4, 5, 6
      json[device_id-1]=device["ssid"];
    }
    String res;
    serializeJson(json, res);
    req->send(200,"application/json",res);
    });
  server.on("/sw",HTTP_POST,[](AsyncWebServerRequest * req){
    Serial.println("sw");
    //getTest();
    //postTest();
    postOnboard();
    req->send(200);
    });
  server.begin();
  Serial.print("Setup()");
  Serial.println(st[status]);
}

void loop() 
{
  switch(status)
  {
    case WIFI_DISCONNECTED:
      if(BT.available())
      {
        Serial.println(st[status]);
        val=BT.read();
        switch(val)
        {
          case 'h': 
            Serial.println("help");
            BT.println("type the following command to setup");
            BT.println("1. scan WIFI again");
            BT.println("2. list WIFI");
            BT.println("3. setup WIFI");
            break;
          case '1':
            Serial.println("scan WIFI");
            BT.println("scan WIFI");
            scanWIFI();
            break;
          case '2':
            Serial.println("list WIFI");
            BT.println("list WIFI");
            listWIFI();
            break;
          case '3':
            Serial.println("setup WIFI");
            BT.println("setup WIFI");
            Serial.println("Select the following SSID");
            BT.println("Select the following SSID");
            listWIFI();
            status=WIFI_SETUP_SSID;
            break;
          default:
            Serial.println("default");
            BT.println("type h to get help");
            break;
        }
      }
      break;
    case WIFI_SETUP_SSID: //get SSID from wifi scan list
      if(BT.available())
      {
        Serial.println(st[status]);
        val=BT.read();
        switch (val)
        {
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
            selectSSID(String(val).toInt());
            status=WIFI_SETUP_SSID_TO_WIFI_SETUP_PASSWORD;
            break;
          default:
            Serial.println("Please input 1~6 to select SSID");
            BT.println("Please input 1~6 to select SSID");
            listWIFI();
            break;
        }
      }
      break;  
    case WIFI_SETUP_SSID_TO_WIFI_SETUP_PASSWORD:
      Serial.println("input password:");
      BT.println("input password:");
      status=WIFI_SETUP_PASSWORD;
      break;
    case WIFI_SETUP_PASSWORD: //get password from bt connection
      if(BT.available())
      {
        Serial.println(st[status]);
        strcpy(password,BT.readString().c_str());
        Serial.printf("Password:%s\n",password);
        BT.printf("Password:%s\n",password);
        status=WIFI_SETUP;
      }
      break;
    case WIFI_SETUP:
      Serial.println(st[status]);
      WiFi.begin(ssid,password);
      while(WiFi.status()!=WL_CONNECTED)
      {
        Serial.print(".");
        delay(500);
      }
      Serial.printf("\nIP address:%s\n",WiFi.localIP().toString().c_str());
      status=WIFI_CONNECTED;
      setConnected(1);
      setSSID();
      setPassword();
      //printFile(CONFIG_FILE);
      break;
      /*case WIFI_CONNECTED:
        break;*/
    default:
      break;
  }     
}
