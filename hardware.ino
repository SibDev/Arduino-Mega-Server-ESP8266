/*
  Modul Hardware for ESP8266
  part of Arduino Mega Server project
*/

#define MAX_MEMORY 81920
#define MAX_FREE_MEM 46224
#define PERC_FREE_MEM (MAX_FREE_MEM / 100)
#define MAX_PROGRAM 1044464
#define MIN_CKETCH 208368

void hardwareInit() {
  initStart_("Hardware");
  Serial.print(F("SDK version:     ")); Serial.println(ESP.getSdkVersion());
  Serial.print(F("Boot version:    ")); Serial.println(ESP.getBootVersion());
  Serial.print(F("Boot mode:       ")); Serial.println(ESP.getBootMode());
  Serial.print(F("Reset info:      ")); Serial.println(ESP.getResetInfo());
  Serial.print(F("CPU freq:        ")); Serial.print(ESP.getCpuFreqMHz()); Serial.println(" MHz");
  Serial.print(F("Free memory:     "));
  Serial.print(ESP.getFreeHeap());
  Serial.print(F(" ("));
  printFreeMemP();
  Serial.println(F("%)"));
  
  //Serial.printf("Chip ID:         %08X\n", ESP.getChipId());
  //Serial.printf("Flash ID:        %08X\n", ESP.getFlashChipId());
  
  Serial.print(F("Flash speed:     ")); Serial.print(ESP.getFlashChipSpeed() / 1000000); Serial.println(" MHz");
  Serial.print(F("Flash mode:      ")); Serial.println(ESP.getFlashChipMode());
  Serial.print(F("Flash size:      ")); Serial.println(ESP.getFlashChipSize());
  if (ESP.getFlashChipSize() != ESP.getFlashChipRealSize()) {
    Serial.print(F("Flash real size: ")); Serial.println(ESP.getFlashChipRealSize());
  }
  Serial.print(F("SPIFFS size:     ")); Serial.println(ESP.getFlashChipRealSize() - (ESP.getSketchSize() + ESP.getFreeSketchSpace()));
  Serial.print(F("Program size:    ")); Serial.println(ESP.getSketchSize() + ESP.getFreeSketchSpace());
  Serial.print(F("Sketch size:     ")); Serial.println(ESP.getSketchSize());
  Serial.print(F("Free for sketch: ")); Serial.println(ESP.getFreeSketchSpace());
  initDone_();
}

void printFreeMemP() {
  Serial.print(ESP.getFreeHeap() / PERC_FREE_MEM);
}

int freeMem() {
  return ESP.getFreeHeap();
}

// CPU load
// ESP-12F, 80 MHz, QIO 40 MHz

// cyclos in sec
long cyclosInSec  = 0;
long cyclosInSecP = 0;

void calcCyclosP() {
  cyclosInSecP = 100 - (cyclosInSec / 10);
  if (cyclosInSecP <   0) {cyclosInSecP =   0;}
  if (cyclosInSecP > 100) {cyclosInSecP = 100;}
  cyclosInSec = 0;
}

void cyclosInSecWork() {
  cyclosInSec++;
  if (cycle1s) {calcCyclosP();}
}

// cyclos delay
#define MAX_CPU_BUFFER 10
unsigned long bufferCpuLoad[MAX_CPU_BUFFER];
unsigned long oldCycle = 0;

byte cyclosDelayP() {
  unsigned long summ = 0;
  for (byte i = 0; i < MAX_CPU_BUFFER; i++) {summ += bufferCpuLoad[i];}
  int cyclosDelay = summ / MAX_CPU_BUFFER; // delay >= 100 ms (100% load)
  cyclosDelay /= 2; //  k=10 => delay >= 1 s (100% load)
  if (cyclosDelay <   0) {cyclosDelay =   0;}
  if (cyclosDelay > 100) {cyclosDelay = 100;}
  return cyclosDelay;
}

void cyclosDelayWork() {
  unsigned long now2 = millis();
  for (byte i = MAX_CPU_BUFFER - 1; i > 0; i--) {
    bufferCpuLoad[i] = bufferCpuLoad[i - 1];
  }
  bufferCpuLoad[0] = now2 - oldCycle;
  oldCycle = now2;
}

