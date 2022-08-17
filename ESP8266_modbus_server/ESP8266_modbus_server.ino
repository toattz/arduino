/* 
 * ��蝐喟�烐� 2017/3/30 http://www.icdt.com.tw 
 * 雿輻鍂 ESP8266 (NodeMCU) 雿輻鍂 Arduino �抅�䲰 MODBUS TCP ��蝭�靘讠�见��
 * 撖血�帋� Function 3/4 & Function 6
 * �瑕�� 64 �� Register(40001~40064)嚗𪄇ID 0~255���虾
 * 蝭�靘衤蝙�鍂 ��� 6 �� Register �綉�� 6 �� Relay嚗���ê̌�𦻖�秐 GPIO 5,4,2,14,12,13��銋笔停�糓 NodeMCU ��D1,D2,D4,D5,D6 & D7
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <stdio.h>


const char* ssid = "ZYXEL_1F";
const char* pass = "C1470A9A";

//modbus �㺭撣訾誨蝣�
#define   ILLEGAL_FUNCTION      1
#define   ILLEGAL_DATA_ADDRESS  2
#define   ILLEGAL_DATA_VALUE    3
#define   SLAVE_DEVICE_FAILURE  4
#define   ACKNOWLEDGE           5
#define   SLAVE_DEVICE_BUSY     6

//modbus tcp port
#define   MODBUS_Port 502

//��鞱身 Register �彍���
#define   Register_Nb 64

//GPIO �彍���
#define   gpioNb 6   

//Relay 頛詨枂��滚��(閬� Relay �𤓖頝航����)
#define   OutputReverse

//UART 憿舐內 WIFI 閮𦠜��
#define printWifi

//NodeMCU LED �喃��
#define   red_LED 16  

//摰𡁶儔 Relay GPIO �喟楊���
//�㮾�訜 NodeMCU D1,D2,D4,D5,D6,D7
uint8_t gpioPin[6]={5,4,2,14,12,13};

//Register 閮䀹�園腼���
uint16_t Register[Register_Nb];

unsigned long systemtime;
bool ledFlag = false;

// IP 閮剖�𡄯�������� AP �㮾����蝬�
/*IPAddress myIp(192,168,0,133);
IPAddress gw(192,168,0,1);
IPAddress subnet(255,255,255,0);*/

//�朞�𡃏撓��/頛詨枂蝺抵�嘥�
//buffer to hold incoming and outgoing packets
uint8_t rxBuffer[128]; 
uint8_t txBuffer[256];

WiFiServer server(MODBUS_Port);

//��嘥�贝身摰�
void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  pinMode(red_LED, OUTPUT);      // sets the digital pin as output
  for(int i=0;i<gpioNb;i++)
  {
    pinMode(gpioPin[i], OUTPUT);      // sets the digital pin as output
#ifdef OutputReverse
    digitalWrite(gpioPin[i], HIGH);  
#endif
  }
  digitalWrite(red_LED, HIGH);   
  setupWiFi_ST();
  // Start the TCP server
  server.begin();
  systemtime = millis();
  Serial.printf("LED_BUILTIN:%d\n",LED_BUILTIN);
}

//銝餉艘���
void loop() {
  unsigned long looptime = millis();
  //�⊿���𡄯�峕�� 2 Sec 暺硺漁��𣇉��皛� LED
  if(looptime >= systemtime + 2000)
  {
    systemtime = looptime;
    if(ledFlag)
      digitalWrite(red_LED, LOW);   
    else
      digitalWrite(red_LED, HIGH); 
    ledFlag = !ledFlag;   
  }
  TCPServer();
}

//憿舐內 WIFI ���𡁶���
void printWifiStatus() {
#ifdef printWifi
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
#endif
}

//WIFI ����
void setupWiFi_ST(){
  //WiFi.config(myIp,gw,subnet);
  // setting up Station AP
  WiFi.begin(ssid, pass);
  
  // Wait for connect to AP
  Serial.print("[Connecting]");
  Serial.print(ssid);
  int tries=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tries++;
    if (tries > 30){
      break;
    }
  }
  Serial.println();
  printWifiStatus();
}

//MODBUS TCP ��墧�厩㺭撣貉�𦠜��
int ErrorMessage(uint8_t code)
{
  txBuffer[0] = rxBuffer[0];        //Transaction Identifier
  txBuffer[1] = rxBuffer[1];        //Transaction Identifier
  txBuffer[2] = rxBuffer[2];        //Protocol Identifier
  txBuffer[3] = rxBuffer[3];        //Protocol Identifier
  txBuffer[4] = 0;                  //Length
  txBuffer[5] = 3;                  //Length
  txBuffer[6] = rxBuffer[6];        //Unit Identifier
  txBuffer[7] = rxBuffer[7] | 0x80; //Function
  txBuffer[8] = code;               //Exception Code
  return 9;
}

//MODBUS TCP 鞈��坔神�枂嚗�銁甇斗凒�鰵頛詨枂
void DataWrite(uint16_t Addr, uint16_t value)
{
#ifdef OutputReverse
  bool isON = value==0;
#else
  bool isON = value>0;
#endif

  if(Addr < gpioNb)
  {
    if(isON)
      digitalWrite(gpioPin[Addr], HIGH);  
    else
      digitalWrite(gpioPin[Addr], LOW);  
  }
}

//MODBUS TCP ��墧�㚁��銁甇文�� FUNC 3 & & 6 ��㗇�� 嚗��園�䁅�贝䌊銵�祕雿�
int Modbus_Tcp_Resp(int len)
{
  uint8_t Func= rxBuffer[7];
  Serial.printf("Func=%d\n",Func);
  int result = 0;
  uint16_t RegAdd = (rxBuffer[8]<<8) | rxBuffer[9];
  uint16_t RegLen = (rxBuffer[10]<<8) | rxBuffer[11];
  Serial.printf("RegAdd:%d RegLen:%d\n RegAdd:%x RegLen:%x\n",RegAdd,RegLen,RegAdd,RegLen);
  if((Func ==3) || (Func ==4))  //03 Read Holding Registers;04 Read Input Registers
  {
    if((RegAdd + RegLen) <=Register_Nb)
    {
      result = RegLen *2 + 9;
      Serial.printf("RegLen:%d\n",RegLen);
      txBuffer[0] = rxBuffer[0];        //Transaction Identifier
      txBuffer[1] = rxBuffer[1];        //Transaction Identifier
      txBuffer[2] = rxBuffer[2];        //Protocol Identifier
      txBuffer[3] = rxBuffer[3];        //Protocol Identifier
      txBuffer[4] = (result -6) >>8;    //Length
      txBuffer[5] = (result -6)  & 0xff;//Length
      txBuffer[6] = rxBuffer[6];        //Unit Identifier
      txBuffer[7] = rxBuffer[7];        //Function
      txBuffer[8] = RegLen *2;          //Byte Count
      Serial.printf("txBuffer[4]:%d txBuffer[5]:%d\ntxBuffer[4]:%x txBuffer[5]:%x\n",txBuffer[4],txBuffer[5],txBuffer[4],txBuffer[5]);
      for(int i=0;i<RegLen;i++)
      {
        txBuffer[9+i*2] = Register[RegAdd]>>8;
        txBuffer[10+i*2] = Register[RegAdd] & 0xff;
        RegAdd++;
      }
      Register[63]++;
    }
    else
      result = ErrorMessage(ILLEGAL_DATA_ADDRESS);
  }
  else if(Func ==6) //06 Preset Single Register
  {
    if(RegAdd <Register_Nb)
    {
      Register[RegAdd] = RegLen;
      DataWrite(RegAdd,RegLen);
      for(int i=0;i<len;i++)
        txBuffer[i] = rxBuffer[i];
      result = len;   
    }
  }
  else  //Error
    result = ErrorMessage(ILLEGAL_FUNCTION);  //Exception Codes : ILLEGAL FUNCTION
  return result;
}

//MODBUS TCP Server 蝔见��
void TCPServer () {
    WiFiClient client = server.available();
    int nxCnt =0;
    while (client.connected())
    {
      if (client.available() > 0) {
      // Read incoming message
      digitalWrite(red_LED, LOW);   
      int len = client.read(rxBuffer, 128);
      for(int i=0;i<len;i++)
      {
        Serial.printf("rxBuffer[%d]:%d  rxBuffer[%d]:%x\n",i,rxBuffer[i],i,rxBuffer[i]);
      }

      if(len >0)
      {
        Serial.printf("rxBuffer[4]:%d rxBuffer[4]<<8:%d rxBuffer[5]:%d (rxBuffer[4]<<8 | rxBuffer[5]):%d\n",rxBuffer[4],rxBuffer[4]<<8,rxBuffer[5],(rxBuffer[4]<<8 | rxBuffer[5]));
        Serial.printf("rxBuffer[4]:%x rxBuffer[4]<<8:%x rxBuffer[5]:%x (rxBuffer[4]<<8 | rxBuffer[5]):%x\n",rxBuffer[4],rxBuffer[4]<<8,rxBuffer[5],(rxBuffer[4]<<8 | rxBuffer[5]));
        if((len == ((rxBuffer[4]<<8 | rxBuffer[5]) + 6))
          && (rxBuffer[2] ==0)&& (rxBuffer[3] ==0) && (len >=12))  //(長度等於bit[4]<<8|bit[5])&&bit[2]==0&&bit[3]==0 長度大於等於12
        {
#ifdef printWifi
          Serial.printf("RX(%d):" , len); 
          for(int i=0;i<len;i++)
            Serial.printf( " %02X", rxBuffer[i] );
          Serial.println("");   
#endif
          int result = Modbus_Tcp_Resp(len);
          if(result >0)
            client.write(txBuffer,result);
          digitalWrite(red_LED, HIGH);   
        }
        else
        {
#ifdef printWifi
        Serial.printf("Err(%d):" , len); 
        for(int i=0;i<len;i++)
          Serial.printf( " %02X", rxBuffer[i] );
        Serial.println("");   
#endif
        }
        nxCnt =0;
      }
    }
    else  //�∟����
    {
      unsigned long looptime = millis();
      if(looptime >= systemtime +500)
      {
        systemtime = looptime;
        if(ledFlag)
          digitalWrite(red_LED, LOW);   
        else
          digitalWrite(red_LED, HIGH); 
        ledFlag = !ledFlag;  
        nxCnt++;
        if(nxCnt >=100) // 頞��� 50 蝘垍�∟���嗘葉�𪃾 TCP ����
        {
           client.stop();
           Serial.println("Timeout!!");   
        }
      }
    }
  }
}

  
