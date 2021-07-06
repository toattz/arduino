#include<BLEDevice.h>
#include<BLEServer.h>
#include<BLEUtils.h>
#include<BLE2902.h>

/*#define SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID_TX "11111111-2222-3333-4444-555555555555"
#define CHARACTERISTIC_UUID_RX "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"*/

#define SERVICE_UUID           "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_RX "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_TX "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
#define BATT_UUID (uint16_t)0x180F

uint8_t battLevel=100;
const int LED=25;
bool bleConnected=false;
BLECharacteristic *pCharact_TX;
BLECharacteristic *pCharactBatt;

class RXCallbacks:public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharact)
  {
    Serial.println("RXCallbacks");
    std::string rxVal=pCharact->getValue();

    if(rxVal=="on")
    {
      Serial.println("turn on the light");
      digitalWrite(LED_BUILTIN,HIGH);
    }
    else if(rxVal=="off")
    {
      Serial.println("turn off the light");
      digitalWrite(LED_BUILTIN,LOW);
    }
  }
};

class ServerCallbacks:public BLEServerCallbacks
{
  void onConnect(BLEServer * pServer)
  {
    bleConnected=true;
  }

  void onDisconnect(BLEServer *pServer)
  {
    bleConnected=false;
    Serial.println("Disconnected");
    BLEDevice::startAdvertising();
  }
};

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN,OUTPUT);
  
  BLEDevice::init("ESP32 BLE test");
  BLEServer *pServer=BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  //TX RX Service
  BLEService *pService=pServer->createService(SERVICE_UUID);
  pCharact_TX=pService->createCharacteristic(CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY|BLECharacteristic::PROPERTY_READ);
  BLECharacteristic *pCharact_RX=pService->createCharacteristic(CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);
  pCharact_TX->addDescriptor(new BLE2902());
  pCharact_RX->setCallbacks(new RXCallbacks());
  BLEDescriptor *pDesc=new BLEDescriptor((uint16_t)0x2901);
  pDesc->setValue("Control BLE LED");
  pCharact_RX->addDescriptor(pDesc);
  pService->start();

  //Battery Service
  BLEService *pBatteryService=pServer->createService(BATT_UUID);
  pCharactBatt=pBatteryService->createCharacteristic((uint16_t)0x2A19,BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor *pBattDesc=new BLEDescriptor((uint16_t)0x2901);
  pBattDesc->setValue("Volume 0~100%");
  pCharactBatt->addDescriptor(pBattDesc);
  pCharactBatt->addDescriptor(new BLE2902());
  pBatteryService->start();
  
  pServer->getAdvertising()->start();

  Serial.println("Wait client to connect");
}

void loop() 
{
  if(bleConnected)
  {
    int hallVal=hallRead();
    char buffer[5];
    itoa(hallVal,buffer,10);
    pCharact_TX->setValue(buffer);
    pCharact_TX->notify();
    Serial.printf("Send notify:%d\n",hallVal);

    pCharactBatt->setValue(&battLevel,1);
    pCharactBatt->notify();
    delay(1000);
    battLevel--;
    if(battLevel==0)
    {
      battLevel=100;
    }
  }
  
  delay(1000);
}
