#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
 
AsyncWebServer server(80);
 
// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ZYXEL_1F";
const char* password = "C1470A9A";
 
int a = 0;
int a1 = 90;
int a2 = 90;
int a3 = 90;
int a4 = 90;
int a5 = 90;
String an1 = "";
String an2 = "";
String an3 = "";
String an4 = "";
String an5 = "";
const char* PARAM_INPUT = "input";
String test = "off";
String inputMessage;
String inputParam;
 
Servo myservo;  // 建立SERVO物件
 
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}</style>
  <meta charset="utf-8">
  </head><body>
  <h1>ESP32 Web Server</h1>
  <h3>程式用法介紹</h3>
  <h4>請在下方輸入角度值1~180
  <br>提交後欲更改請按"Return to Home Page"
  <br>欲儲存請按save按鈕
  <br>conduct後將會清空資料</h4>
  <form action="/get">
    input: <input type="number" min="0" max="180" name="input">
    <input type="submit" value="提交">
    <br><input type="reset" value="清空輸入欄">
  </form>
  <br><p><a href="/conduct/to"><button class="button">執行</button></a></p>
</body></html>)rawliteral";
 
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
 
void setup() {
  Serial.begin(115200);
 
  pinMode(32, OUTPUT);  //這兩行請改成自己的馬達腳位，預設為32
  myservo.attach(32);
  myservo.write(90);    //旋轉到90度位置為歸零
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");    
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    myservo.write(90); //旋轉到90度，歸零
 
    request->send_P(200, "text/html", index_html);
  });
 
  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET input value on <ESP_IP>/get?input=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      inputParam = PARAM_INPUT;
    }else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    a = inputMessage.toInt();
    myservo.write(a);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>"
                                     + "<br><p><a href=\"/save/to\"><button class=\"button\">SAVE</button></a></p>");
  });
 
  server.on("/save", HTTP_GET, [] (AsyncWebServerRequest *request){
 
    if (a5 == 90){
      if (a4 == 90){
        if (a3 == 90){
          if (a2 == 90){
            if (a1 == 90){
              an1 = inputMessage;
              a1 = a;
            }else{
            an2 = inputMessage;
            a2 = a;
            }
          }else{
          an3 = inputMessage;
          a3 = a;
          }
        }else{
          an4 = inputMessage;
          a4 = a;
        }
      }else{
        an5 = inputMessage;
        a5 = a;
      }
    }else{
  /*<br><p><a href="/conduct/to"><button class="button">CONDUCT</button></a></p>         這裡是第一個問題
    "conduct後將會清空資料"*/
    }
 
    request->send(200, "text/html", "angel 1 =" + an1 + 
                                    "<br>angel 2 =" + an2 +
                                    "<br>angel 3 =" + an3 +
                                    "<br>angel 4 =" + an4 +
                                    "<br>angel 5 =" + an5 +
                                    "<br><a href=\"/\">Return to Home Page</a>");
    });
 
    server.on("/conduct", HTTP_GET, [] (AsyncWebServerRequest *request){
 
      myservo.write(a1);
      delay(500);
      myservo.write(a2);
      delay(500);
      myservo.write(a3);
      delay(500);
      myservo.write(a4);
      delay(500);
      myservo.write(a5);
      delay(500);
      a1 = 90;
      a2 = 90;
      a3 = 90;
      a4 = 90;
      a5 = 90;
      an1 = "";
      an2 = "";
      an3 = "";
      an4 = "";
      an5 = "";
      request->send(200, "text/html", /*"<br>now conduct" +    //這裡是第二個問題
                                    "<br>angel 1 =" + an1 + 
                                    "<br>angel 2 =" + an2 +
                                    "<br>angel 3 =" + an3 +
                                    "<br>angel 4 =" + an4 +
                                    "<br>angel 5 =" + an5 +
                                    "<br>completed" +
                                    "<br>the volues have been reset" +*/
                                    "<br><a href=\"/\">Return to Home Page</a>");
 
      });
  server.onNotFound(notFound);
  server.begin();
}
 
void loop() {
 
}
