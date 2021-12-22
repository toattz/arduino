server.on("/post", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
      }
      int params = request->params();
      Serial.println("Do I even get here ?!");
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost() && p->value() != NULL) {
          Serial.println((String)"POST[" + p->name().c_str() + "]: " + p->value().c_str());
          if(p->name() == "number") {
            licensePlate = p->value();
            Serial.println("License plate POST: " + licensePlate);
            change = true;
          }
        }
      } // for(int i=0;i<params;i++)
      Serial.println(); 
      request->send(200);
  });

  server.onNotFound([](AsyncWebServerRequest *request){ request->redirect("/home"); });


    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest * request) {
 
    if (ON_STA_FILTER(request)) {
      request->send(200, "text/plain", "Hello from STA");
      return;
 
    } else if (ON_AP_FILTER(request)) {
      request->send(200, "text/plain", "Hello from AP");
      return;
    }
 
    request->send(200, "text/plain", "Hello from undefined");
  });



  server.on("/log",HTTP_GET,[](AsyncWebServerRequest *req){
    //String logs="";
    File file=SD.open("log.txt","r");
    if(!file)
    {
      Serial.println("open file fail");
      return;
    }
    else
    {
      Serial.println("open file success");
    }
    /*while(file.available())
    {
      logs+=(char)file.read();
    }
    file.close();
    Serial.println(logs.c_str());*/
    req->send(file,"/log.txt","text/xhr");
  });    