/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

//#include <WiFi.h>
#include <ESPAsync_WiFiManager.h>
AsyncWebServer webServer(80);
//const char* ssid     = "ZYXEL_1F";
const char* ssid     = "TP-LINK_470A9A";
const char* password = "C1470A9A";

const char* host = "data.sparkfun.com";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup()
{
    Serial.begin(115200);
    Serial.print("\nStarting Async_AutoConnect_ESP32_minimal on " + String(ARDUINO_BOARD)); Serial.println(ESP_ASYNC_WIFIMANAGER_VERSION);
    ESPAsync_WiFiManager ESPAsync_wifiManager(&webServer, NULL, "Async_AutoConnect");
    ESPAsync_wifiManager.resetSettings();
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      
        delay(1000);
        if(WiFi.status()==WL_CONNECTED)
        {
          Serial.println("WL_CONNECTED");
        }
        else if(WiFi.status()==WL_NO_SHIELD)
        {
          Serial.println("WL_NO_SHIELD");
        }
        else if(WiFi.status()==WL_IDLE_STATUS)
        {
          Serial.println("WL_IDLE_STATUS");
        }
        else if(WiFi.status()==WL_NO_SSID_AVAIL)
        {
          Serial.println("WL_NO_SSID_AVAIL");
        }
        else if(WiFi.status()==WL_SCAN_COMPLETED)
        {
          Serial.println("WL_SCAN_COMPLETED");
        }
        else if(WiFi.status()==WL_CONNECT_FAILED)
        {
          Serial.println("WL_CONNECT_FAILED");
        }
        else if(WiFi.status()==WL_CONNECTION_LOST)
        {
          Serial.println("WL_CONNECTION_LOST");
        }
        else if(WiFi.status()==WL_DISCONNECTED)
        {
          Serial.println("WL_DISCONNECTED");
        }
        else
        {
          Serial.println("else");
        }
        
        Serial.printf("%d.",WiFi.status());
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int value = 0;

void loop()
{
    delay(5000);
    ++value;

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/input/";
    url += streamId;
    url += "?private_key=";
    url += privateKey;
    url += "&value=";
    url += value;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}
