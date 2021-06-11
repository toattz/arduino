#include<SPI.h>
#include<Ethernet.h>
#include<WebServer.h>
#include<Streaming.h>
#include<DHT.h>

DHT dht(2,DHT11);

byte mac[]={0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};
/*IPAddress ip(192,168,1,126);
IPAddress subnet(255,255,255,0);
IPAddress gateway(192,168,1,1);*/
const int gLed=6,yLed=7,maxDebounce=500;
int gLedStatus=0,yLedStatus=0; //LED pins
unsigned int debounce=0;
const int b1=5,b2=9,b3=8;//buttin pins
WebServer webserver("",80);
String addr="";


P(htmlHead)=
  "<!DOCTYPE html>"
  "<html>"
  "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\" />"
  "<title>2 LEDs demo with Webduino</title></head>"
  "<body>"
  "<div id=\"addr\"></div>";

P(htmlHeadRefresh)=
  "<!DOCTYPE html>"
  "<html>"
  "<head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"2\">"
  "<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\"sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\" />"
  "<title>2 LEDs demo with Webduino</title></head>"
  "<body>"
  "<div id=\"addr\"></div>";
  
P(formHead)=
  "<form method=\"post\" action=\"sw\">";

P(formGOff)=
  "Green Light:<input name=\"glight\" type=\"radio\" value=\"on\">ON"
  "<input name=\"glight\" type=\"radio\" value=\"off\" checked>OFF";

P(formGOn)=
  "Green Light:<input name=\"glight\" type=\"radio\" value=\"on\" checked>ON"
  "<input name=\"glight\" type=\"radio\" value=\"off\">OFF";

P(formYOff)=
  "Yellow Light:<input name=\"ylight\" type=\"radio\" value=\"on\">ON"
  "<input name=\"ylight\" type=\"radio\" value=\"off\" checked>OFF";

P(formYOn)=
  "Yellow Light:<input name=\"ylight\" type=\"radio\" value=\"on\" checked>ON"
  "<input name=\"ylight\" type=\"radio\" value=\"off\">OFF";

P(formFooter)=
  "<br><br>"
  "<input type=\"submit\" name=\"button\" value=\"submit\">";

P(htmlFooter)=
  "</boyd></html>";
  
P(jQuery)=
  "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>";

P(script)=
"<script>"
  "var addr=document.getElementById('addr');"
  "addr.innerHTML='<a href=http://'+window.location.hostname+'/ \\ >HOME</a> <a href=http://'+window.location.hostname+'/sw \\ >SW</a>  <a href=http://'+window.location.hostname+'/dht11 \\ >DHT11</a>';"
"</script>";
/*
<script>
  var addr=document.getElementById('addr');
  addr.innerHTML="<a href=\"http://"+windlocation.hostname+"/ \">HOME</a>"+"<a href=\"http://"+location.hostname+"/sw \">SW</a>"+"<a href=\"http://"+location.hostname+"/dht11 \">dht11</a>"
</script>
*/
String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}  

void defaultCmd(WebServer &server,WebServer::ConnectionType type)
{ 
  gLedStatus=digitalRead(gLed);
  yLedStatus=digitalRead(yLed);
  server.httpSuccess();
  
  if(type!=WebServer::HEAD)
  {
    server.printP(htmlHeadRefresh);
    //server.printP(htmlHead);
    server<<"<h1>Green LED:";
    if(gLedStatus==LOW)
    {
      server<<"off"; 
    }
    else
    {
      server<<"on";
    }

    server<<"<br>Yellow LED:";
    if(yLedStatus==LOW)
    {
      server<<"off"; 
    }
    else
    {
      server<<"on";
    }
    server<<"</h1>";
  }
  server.printP(htmlFooter);
  server.printP(script);
}

void dht11Cmd(WebServer &server,WebServer::ConnectionType type)
{
  float h=dht.readHumidity(),t=dht.readTemperature();
  server.httpSuccess();
  
  if(type!=WebServer::HEAD)
  {
    server.printP(htmlHeadRefresh);
    server<<"<h1>Humidity:";
    if(isnan(h))
    { 
      server<<"Nan";
    }
    else
    {
      server<<h;
    }
    server<<"<br>Temperature:";
    if(isnan(t))
    {
      server<<"Nan";
    }
    else
    {
      server<<t;
    }
    server.printP(htmlFooter);
    server.printP(script);
  }
}

void postCmd(WebServer &server,WebServer::ConnectionType type)
{
  char name[16],value[16];
  bool gLightStatus=false,yLightStatus=false; 
  server.httpSuccess();

  if(type==WebServer::POST||type==WebServer::GET)
  { 
    if(type==WebServer::POST)
    {   
      while(server.readPOSTparam(name,16,value,16))
      {
        if(strcmp(name,"glight")==0)
        {
          if(strcmp(value,"on")==0)
          {
            digitalWrite(gLed,HIGH);
            gLightStatus=true;
          }
          else
          {
            digitalWrite(gLed,LOW);
          }
        }
        else if(strcmp(name,"ylight")==0)
        {
          if(strcmp(value,"on")==0)
          {
            digitalWrite(yLed,HIGH);
            yLightStatus=true;
          }
          else
          {
            digitalWrite(yLed,LOW);
          }
        }
      }
    }
    else
    {
      gLightStatus=digitalRead(gLed);
      yLightStatus=digitalRead(yLed);
    }
    server.printP(htmlHead);
    server.printP(formHead);
    server<<"<p>Green Light:";
    if(gLightStatus)
    {
      server<<"ON</p>";
      server.printP(formGOn);
    }
    else
    {
      server<<"OFF</p>";
      server.printP(formGOff);
    }
    server<<"<br><br>";
    server<<"<p>Yellow Light:";
    if(yLightStatus)
    {
      server<<"ON</p>";
      server.printP(formYOn);
    }
    else
    {
      server<<"OFF</p>";
      server.printP(formYOff);
    }
    server.printP(formFooter);
    server.printP(htmlFooter);
    server.printP(script);
  }
}

/*void faqCmd(WebServer &server,WebServer::ConnectionType type)
{
  server.httpSuccess();
  if(type!=WebServer::HEAD)
  {
    server.printP(faqPage);
  }
}*/
void setup()
{
  dht.begin();
  pinMode(gLed,OUTPUT);
  pinMode(yLed,OUTPUT);
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  digitalWrite(gLed,LOW);
  digitalWrite(yLed,LOW);
  Serial.begin(115200);
  //Ethernet.begin(mac,ip,subnet,gateway);
  Ethernet.begin(mac);
  IPAddress ip=Ethernet.localIP();
  addr=IpAddress2String(ip);
  webserver.setDefaultCommand(&defaultCmd);
  webserver.addCommand("sw",&postCmd);
  webserver.addCommand("dht11",&dht11Cmd);
  webserver.begin();
  Serial.print("IP address is:");
  Serial.println(ip);
}

void loop()
{
  gLedStatus=digitalRead(gLed);
  yLedStatus=digitalRead(yLed);

  if(digitalRead(b1)==LOW && !gLedStatus!=digitalRead(gLed))
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(b1)==LOW){}
      debounce=0;
      digitalWrite(gLed,!gLedStatus);
    }
    debounce++;
  }

  if(digitalRead(b2)==LOW && !yLedStatus!=digitalRead(yLed))
  {
    if(debounce>maxDebounce)
    {
      while(digitalRead(b2)==LOW){}
      debounce=0;
      digitalWrite(yLed,!yLedStatus);
    }
    debounce++;
  }

  webserver.processConnection();
}
