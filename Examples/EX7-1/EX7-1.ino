#include <DS18B20.h>
#include <Wire.h>
#define sensorPin 5
OneWire  tempSensor(sensorPin);  
#define arduinoAdr 0x11
int number = 0;
volatile int tempRead = 0;
volatile int tempDigit = 0;
volatile int count = 6;
byte data[12];
volatile float celsius=-1001.0;
volatile bool readyReadTemp = false;
void setup(void) {
  Serial.begin(9600);
  searchSensor(tempSensor);
  Wire.begin(arduinoAdr);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready to get temperature reading!");
}
void loop() {
  if (readyReadTemp) {
    sensorToGo(tempSensor);
    for (int i = 0; i < 9; i++) data[i] = tempSensor.read();
    char errorCode = checkData(data);
    if (errorCode == 1) Serial.println("Check setting");
    else if (errorCode == 2) Serial.println("Data error!");
    else {
      celsius = getTemperature(data);
      Serial.print("Current temperature in Celsius = ");
      Serial.println(celsius,1);
      readyReadTemp = false;    
    }
    delay(10000);
  }
}
void receiveData(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    if (number == 0x73) {
      readyReadTemp = true;     
    }
  }
}
void sendData() {
  int temp1;
  if (celsius < -1000.) {
    Wire.write(0x61);
    return;
  }
  switch (count) {
    case 6:
      tempRead = (int) celsius;
      tempDigit = (int) (celsius*100.0 - floor(celsius)*100.0);
      temp1 = tempDigit%10;
      tempDigit = tempDigit/10;
      Wire.write(temp1);
      count--;
      break;
    case 5:
      temp1 = tempDigit%10;
      Wire.write(temp1);
      count--;
      break;
    case 4:
      Wire.write(0x2E);
      count--;
      break;
    case 0:
      Wire.write(0x65);
      count = 6;
      break;
    default:
      temp1 = tempRead%10;
      tempRead = tempRead/10;
      Wire.write(temp1);
      count--;
      break;
  }
}
