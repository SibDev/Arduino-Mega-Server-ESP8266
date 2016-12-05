/*
  Modul FS for ESP8266
  part of Arduino Mega Server project
  SPIFFS (SPI Flash File System)
*/

void spiffsInit() {
  long count = 0;
  long size = 0;
  
  initStart_("SPIFFS");
  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");

  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    //Serial.printf("%s %d\n", fileName.c_str(), fileSize);
    size += fileSize;
    count++;
  }
  printValue("Total files", String(count));
  printValue("Total size", String(size));
  modulUpload = MODUL_ENABLE;
  initDone_();
} // spiffsInit()

