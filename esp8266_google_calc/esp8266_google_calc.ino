#include<ESP8266WiFi.h>
#include<HTTPSRedirect.h>

const char* ssid = "ZYXEL_1F";
const char* password = "C1470A9A";
const char* host = "script.google.com";
const char *GScriptId = "AKfycbzh5fOUUeZR1Fjd_mw9N2nnel2RhuQnm8EPAYKSBsxF_dT8lX48"; // Google script ID
const int httpsPort = 443;
HTTPSRedirect* client ;
volatile int i=0;
volatile uint32_t count=0;
String url3;

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200); 
  Serial.print("Connecting to wifi: ");
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

  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");

  bool flag = false;
  for (int i=0; i<5; i++)
  { 
    int retval = client->connect(host, httpsPort);
    if (retval == 1)
    {
       flag = true;
       Serial.println("Connected");
       break;
    }
    else
    {
      Serial.println("Connection failed. Retrying...");
    }
  }
  Serial.print("Connecting to ");
  Serial.println(host);
}
void loop()
{
  if(i==60)
  {
    url3="";
    url3 = String("/macros/s/") + GScriptId + "/exec?ip="+WiFi.localIP().toString()+"&router="+ssid; 
    Serial.println("GET Data");
    if (client->GET(url3, host))
    {
      Serial.printf("GET SUCCESS:%u\n",count);
    }
    else
    {
      Serial.printf("GET FAIL:%u\n",count);
    }
    i=0;
    count++;
  }
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
  delay(1000);
  i++;                          
}
