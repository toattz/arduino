#include <ESP8266WiFi.h>
#include <PubSubClient.h>

/***************↓↓↓↓↓ 更改設定值 ↓↓↓↓↓***************/
// WIFI
#define SSID "ZYXEL_1F"
#define WIFIPWD "C1470A9A"

// MQTT
#define MQTTSERVER "io.adafruit.com"
#define MQTTPORT 1883
#define CLIENTID "esptoattz"
#define MQTTUSER "toattz"
#define MQTTPWD "aio_OFMk48CqnbauPPVD3vQSrVDDoCxk"
/*#define IO_USERNAME  "toattz"
#define IO_KEY       "aio_OFMk48CqnbauPPVD3vQSrVDDoCxk"*/

// TOPIC
//#define TOPIC "Adafruit IO 帳號/feeds/Feed Key"
//#define TOPICLIGHT "ssuaiotclass/feeds/lightonoff"
//#define TOPICFAN "ssuaiotclass/feeds/fanonoff"
//#define TOPICKITCHENLIGHT "ssuaiotclass/feeds/kitchenlight"
#define TOPICLIGHT "toattz/feeds/LED"
#define TOPICLIGHT "toattz/feeds/led"
/***************↑↑↑↑↑ 更改設定值 ↑↑↑↑↑***************/

/*int lightpin = 14;        // D5
int fanpin = 12;          // D6
int kitchenlightpin = 13; // D7*/

WiFiClient browser;
PubSubClient mqttClient(browser); // 宣告 mqttClient 物件

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.printf("Message from [%s]\n  Data Length: %d, Payload: ", topic, length);
  Serial.write(payload, length);
  Serial.println("");

  if (strcmp(topic, TOPICLIGHT) == 0)
  { // ssuaiotclass/feeds/lightonoff
    if (memcmp("ON", payload, length) == 0)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);
  }
  /*else if (strcmp(topic, TOPICFAN) == 0)
  { // ssuaiotclass/feeds/fanonoff
    if (memcmp("ON", payload, length) == 0)
      digitalWrite(fanpin, HIGH);
    else
      digitalWrite(fanpin, LOW);
  }
  else if (strcmp(topic, TOPICKITCHENLIGHT) == 0)
  { // ssuaiotclass/feeds/tvonoff
    if (memcmp("ON", payload, length) == 0)
      digitalWrite(kitchenlightpin, HIGH);
    else
      digitalWrite(kitchenlightpin, LOW);
  }*/
}

void mqtt_connect()
{
  if (mqttClient.connect(CLIENTID, MQTTUSER, MQTTPWD))
  {
    Serial.println("MQTT connected");
    mqttClient.subscribe(TOPICLIGHT);
    //mqttClient.subscribe(TOPICFAN);
    //mqttClient.subscribe(TOPICKITCHENLIGHT);
  }
  else
  {
    Serial.print("failed, rc=");
    Serial.print(mqttClient.state());
    Serial.println(" try again in 0.5 seconds");
    delay(500);
  }
}

void setup()
{
  //Declare device pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  /*pinMode(fanpin, OUTPUT);
  digitalWrite(fanpin, LOW);
  pinMode(kitchenlightpin, OUTPUT);
  digitalWrite(kitchenlightpin, LOW);*/

  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, WIFIPWD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MQTT連線設定
  mqttClient.setServer(MQTTSERVER, MQTTPORT); // 連線 MQTT Broker
  mqttClient.setCallback(callback);           // 當 ESP mcu 收到訂閱主題訊息時執行對應函式
}

void loop()
{
  if (!mqttClient.connected())
  {
    //Serial.println("!mqttClient.connected()");
    mqtt_connect();
  }
  else
  {
    //Serial.println("mqttClient.connected()");
  }
}
