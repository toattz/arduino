#include<SPI.h>
#include<Ethernet.h>
byte mac[]={0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
/*IPAddress ip(192,168,1,126);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);*/

EthernetServer server(80);

String HTML=
  "HTTP/1.1 200 OK\r\n"
  "ContentType:text/html\r\n"
  "\r\n"
  "<html><head><meta charset=\"utf-8\"></head>"
  "<body><h1>IOT</h1><body></html>"
  "\r\n";
void setup()
{
  Serial.begin(115200);
  //Ethernet.begin(mac,ip,subnet,gateway);
  Ethernet.begin(mac);
  IPAddress ip=Ethernet.localIP();
  server.begin();
  Serial.print("IP address is:");
  Serial.println(ip);
}

void loop()
{
  EthernetClient client=server.available();
  if(client)
  {
    while(client.connected())
    {
      if(client.available())
      {
        client.print(HTML);
        break;
      }
    }
    delay(1);
    client.stop();
  }
}
