/*
* You need to download mysql library by arudino IDE/Sketch/Include Library/manage Libraries.
* Search "MySQL Connector Arduino" by Dr. Charles Bell, and install it.
*/

#include <WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
//#include <DHT.h>
const char* ssid = "TP-LINK_470A9A_EXT";   // change to your WIFI SSID
const char* password = "C1470A9A";// change to your WIFI Password


IPAddress server_addr(10,1,1,25);  // change to you server ip, note its form split by "," not "."
int MYSQLPort =3306;   //mysql port default is 3306
char user[] = "root";              // Your MySQL user login username(default is root),and note to change MYSQL user root can access from local to internet(%)
char pass[] = "123456";        // Your MYSQL password

WiFiClient client;            
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  //try to connect to WIFI 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //try to connect to mysql server
  if (conn.connect(server_addr, 3306, user, pass)) {
     delay(1000);
     Serial.println("DB Connection success.");
  }
  else{
    Serial.println("DB Connection failed.");
  }  
  delay(2000); 
}



void loop() {
 //insert, change database name and values by string and char[]
  char INSERT_SQL[] = "INSERT INTO test (value) VALUES ('80')";
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  //execute
  if(cur_mem->execute(INSERT_SQL))
  {
    Serial.println("Data Saved is success.");
  }
  else
  {
    Serial.println("Data Saved is fail.");
  }
  delete cur_mem;
  delay(300000);
}
