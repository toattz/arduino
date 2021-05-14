/*
  MySQL Connector/Arduino Example : connect
  This example demonstrates how to connect to a MySQL server from an
  Arduino using an Arduino-compatible Ethernet shield. Note that "compatible"
  means it must conform to the Ethernet class library or be a derivative
  with the same classes and methods.
  
  For more information and documentation, visit the wiki:
  https://github.com/ChuckBell/MySQL_Connector_Arduino/wiki.
  INSTRUCTIONS FOR USE
  1) Change the address of the server to the IP address of the MySQL server
  2) Change the user and password to a valid MySQL user and password
  3) Connect a USB cable to your Arduino
  4) Select the correct board and port
  5) Compile and upload the sketch to your Arduino
  6) Once uploaded, open Serial Monitor (use 115200 speed) and observe
  If you do not see messages indicating you have a connection, refer to the
  manual for troubleshooting tips. The most common issues are the server is
  not accessible from the network or the user name and password is incorrect.
  Note: The MAC address can be anything so long as it is unique on your network.
  Created by: Dr. Charles A. Bell
*/
//#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <WiFi.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const char* ssid = "TP-LINK_470A9A_EXT";   // change to your WIFI SSID
const char* pass = "C1470A9A";// change to your WIFI Password

IPAddress server_addr(10,1,1,25);  // IP of the MySQL *server* here
char user[] = "root";              // MySQL user login username
char password[] = "123456";        // MySQL user login password
WiFiClient client;    
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  while (!Serial); // wait for serial port to connect
  //Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  WiFi.begin(ssid, pass);
  
  //try to connect to WIFI 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  if (client) {
    client.setConnectionTimeout(100);  // set the timeout duration for client.connect() and client.stop()
  }
  if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
    Serial.println("Connection success.");
    // You would add your code here to run a query once on startup.
  }
  else
    Serial.println("Connection failed.");
  conn.close();
}

void loop() {
}
