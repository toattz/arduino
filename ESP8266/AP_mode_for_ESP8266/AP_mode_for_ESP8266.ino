#include <ESP8266WiFi.h>
 
const char *ssid = "Charlie Testing AP";
const char *password = "12345678";
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  Serial.println();
 
  Serial.print("Setting soft-AP ... ");
   
IPAddress softLocal(192,168,128,1);  
IPAddress softGateway(192,168,128,1);
IPAddress softSubnet(255,255,255,0);
 
WiFi.softAPConfig(softLocal, softGateway, softSubnet);  
 
    WiFi.softAP(ssid, password);
   
   IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 server.begin();
 Serial.printf("Web server started, open %s in a web browser\n", WiFi.localIP().toString().c_str());
 
}
 
void loop()
{
 WiFiClient client = server.available();
 if (client)
  {
    Serial.println("\n[Client connected]");
    while (client.connected())
    {
      // read line by line what the client (web browser) is requesting
      if (client.available())
      {
        String line = client.readStringUntil('\r');
        Serial.print(line);
        // wait for end of client's request, that is marked with an empty line
        if (line.length() == 1 && line[0] == '\n')
        {
          client.println(prepareHtmlPage());
 
           
          break;
        }
      }
    }
    delay(10000); // give the web browser time to receive the data
 
    // close the connection:
    client.stop();
    Serial.println("[Client disonnected]");
  }
 
 
}
 
// prepare a web page to be send to a client (web browser)
String prepareHtmlPage()
{
  String htmlPage =
     String("HTTP/1.1 200 OK\r\n") +
            "Content-Type: text/html\r\n" +
            "Connection: close\r\n" +  // the connection will be closed after completion of the response
            "Refresh: 5\r\n" +  // refresh the page automatically every 5 sec
            "\r\n" +
            "<!DOCTYPE HTML>" +
            "<html>" +
            "Analog input:  " + String(analogRead(A0)) +
            "</html>" +
            "\r\n";
  return htmlPage;
}
