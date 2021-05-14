#include <WiFi.h>
#include <PubSubClient.h>


#define WIFI_SSID "YOUR_SSID"    //  your network SSID (name)
#define WIFI_PASSWORD "WPA/WPA2_PASSWORD"    // your network password

#define MQTT_SERVER_IP "10.1.1.27"
#define MQTT_SERVER_PORT 1883
#define MQTT_CLIENT_ID "linklt7697_12345678"
#define MQTT_SUB_TOPIC "javatoybox/linkit7697/AC-Light-switch"

int controlPin = 3;         //set control pin for on/off relay

int status = WL_IDLE_STATUS;      // the Wifi radio's status
WiFiClient mqttClient;
PubSubClient client(mqttClient);

void callback(char* topic, byte* payload, unsigned int length);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // set Pin mode
  pinMode(controlPin, OUTPUT);
  
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback);
  connectWifi();
  connectMQTT();
}

void loop() {
  // check the network connection once every 1 seconds:
  // delay(1000);

  // check if disconnect WIFI, then reconnect
  connectWifi();
  connectMQTT();
  client.loop();
}

void connectWifi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    delay(5000);
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // you're connected now, so print out the data:
    if (status = WL_CONNECTED){
      Serial.println("You're connected to Wifi network");
      printWifiData();
    }
  }
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  char mac_str[18];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.println(mac_str);
  Serial.println("====================");
}

void connectMQTT() {
   // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID))
    {
      Serial.println("connected");
      client.subscribe(MQTT_SUB_TOPIC);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
// Callback function
void callback(char* topic, byte* payload, unsigned int length) {   //MQTT Relay
  Serial.print("Input Message arrived [");
  Serial.print(MQTT_SUB_TOPIC);
  Serial.print("] ");
  Serial.print((char)payload[0]);
  if((char)payload[0] == '1'){
    digitalWrite(controlPin, HIGH);
  }else if((char)payload[0] == '0'){
    digitalWrite(controlPin, LOW);
  }else{
    Serial.print("value error");
  }
  Serial.println();
}
