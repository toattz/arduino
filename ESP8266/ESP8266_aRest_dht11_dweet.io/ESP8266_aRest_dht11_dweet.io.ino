/***************************************************

Written by Marco Schwartz for Open Home Automation.  
BSD license, all text above must be included in any redistribution

Based on the original sketches supplied with the ESP8266/Arduino 
implementation written by Ivan Grokhotkov      

****************************************************/

// Libraries
#include <ESP8266WiFi.h>
#include "DHT.h"

// WiFi parameters
const char* ssid = "ZYXEL_1F";
const char* password = "C1470A9A";

// Pin
#define DHTPIN 5

// Use DHT11 sensor
#define DHTTYPE DHT11

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Host
const char* host = "dweet.io";

void setup() {
  
  // Start Serial
  Serial.begin(115200);
  delay(10);
  
  // Init DHT 
  dht.begin();

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
 
  Serial.print("Connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // Reading temperature and humidity
  int h = dht.readHumidity();
  // Read temperature as Celsius
  int t = dht.readTemperature();
  
  // This will send the request to the server
  client.print(String("GET /dweet/for/blanka671107?temperature=") + String(t) + "&humidity=" + String(h) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  int timeout = millis() + 10000;
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()==0)
  {
    /*if(timeout-millis()<0)
    {
      Serial.println("Client timeout");
      client.stop();
      return;
    }*/
    //String line = client.readStringUntil('\r');
    String line = client.readString();
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  
  // Repeat every 10 seconds
  delay(10000);
 
}
