#include <SPI.h>
#include <SD.h>

File myFile;

// 設定 SD library 功能變數:

Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 4;

void setup() {

  Serial.begin(115200);  // 開啟通訊串列埠開啟
  while (!Serial) {    // 等待串列埠連線
  }

  //----------- 寫入檔案
  Serial.print("\nWaiting for SD card ready...");

  if (!SD.begin(4)) {
    Serial.println("Fail!");
    return;
  }
  Serial.println("Success!");

  myFile = SD.open("card.txt", FILE_WRITE);       // 開啟檔案，一次僅能開啟一個檔案
  
  if (myFile) {                                   // 假使檔案開啟正常
    Serial.print("Write to card.txt...");         
    myFile.println("Test to write data to SD card...");  // 繼續寫在檔案後面
    myFile.close();                               // 關閉檔案
    Serial.println("Completed!");
  } else {
    Serial.println("\n open file error ");    // 無法開啟時顯示錯誤
  }
  
   //----------- 顯示SD 卡資訊

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Check: SD Card");
    return;
  } else {
    Serial.println("============= Card Information ==================");
  }

  // 顯示SD卡類型

  Serial.print("Card type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknow");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.");
    return;
  }

  // 顯示類型和 FAT 空間大小
  uint32_t volumesize;
  Serial.print("Volume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);

  Serial.println("================= Finished =====================");
}

void loop() {            // 在Setup()執行完成後，就等結束 
}
