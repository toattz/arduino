#include <DS18B20.h>
byte bufferSensor[8];
char *errorMsg[]={"Sensor OK","CRC not OK","Not DS18B20","Data not OK"};
char* searchSensor(OneWire tempSensor) {
  while ( !tempSensor.search(bufferSensor)) {
    tempSensor.reset_search();
    delay(250);
  }
  int codeNo=0;
  if (OneWire::crc8(bufferSensor, 7) != bufferSensor[7]) {
    codeNo=1;
  }
  if (bufferSensor[0] != 0x28) {
    codeNo=2;
  }
  return errorMsg[codeNo];
}
void sensorToGo(OneWire tempSensor) {
  tempSensor.reset();
  tempSensor.select(bufferSensor);
  tempSensor.write(0x44, 1);        
  delay(1000); 
  tempSensor.reset();
  tempSensor.select(bufferSensor);    
  tempSensor.write(0xBE);        
}
char* checkData(byte *data) {
  int codeNo=0;
  if (OneWire::crc8(data, 8) != data[8]) codeNo=3;
  return errorMsg[codeNo];
}
float getTemperature(byte *data) {
  float temp = 0.0;
  int16_t rawData;
  rawData = ((data[1] << 4) | (data[0] >> 4)) & 0x07FF;
  temp = (float) rawData ;
  if ((data[1] & 0x08) == 0x08) temp = -temp;
  if ((data[0] & 0x08) == 0x08) temp = temp + 0.5;
  if ((data[0] & 0x04) == 0x04) temp = temp + 0.25;
  if ((data[0] & 0x02) == 0x02) temp = temp + 0.125;
  if ((data[0] & 0x01) == 0x01) temp = temp + 0.0625;
  return temp;
}