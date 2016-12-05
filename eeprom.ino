/*
  Modul EEPROM for ESP8266
  part of Arduino Mega Server project
  
  EEPROM.commit();
*/

#include <EEPROM.h>

#define EEPROM_LENGTH 2048
int eeVolume = EEPROM_LENGTH;
byte eeAddress;

#define EE_ADR_ADDRESS  100
#define EE_ID_ADR       102
#define EE_ID_LEN       15
#define EE_ID_CHECK_ADR 117
#define EE_ID_CHECK_VAL 231
#define EE_NAME_ADR     118
#define EE_NAME_LEN     15

// EEPROM init

void eepromInit() {
  initStart_("EEPROM");
 
  if (!eeCheckName()) {
    eeSetName(SELF_NAME);
  }

  eeAddress = eeGetAddress();
  if (eeAddress == 0 || eeAddress == 255) {
    //eeSetAddress(random(1, 255)); // seed problem
    //eeAddress = eeGetAddress();
  }
  
  eeCheckId(); // seed problem

  #ifdef SERIAL_PRINT
    printEeInfo();
  #endif
  initDone_();
}

// Address

void eeSetAddress(byte addr) {
  EEPROM.begin(EEPROM_LENGTH);
    EEPROM.write(EE_ADR_ADDRESS, addr);
  EEPROM.end();
}

byte eeGetAddress() {
  EEPROM.begin(EEPROM_LENGTH);
    return EEPROM.read(EE_ADR_ADDRESS);
  EEPROM.end();
}

// Name

boolean eeCheckName() {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = EE_NAME_ADR; i < EE_NAME_ADR + EE_NAME_LEN; i++) {
      if (EEPROM.read(i) != 0 && EEPROM.read(i) != 255) {
        EEPROM.end();
        return true;
      }
    }
  EEPROM.end();
  return false;
}

void eeSetName(String s) {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = EE_NAME_ADR; i < EE_NAME_ADR + EE_NAME_LEN; i++) {
      if (i - EE_NAME_ADR > s.length()) {
        EEPROM.write(i, 0);
      } else {
          EEPROM.write(i, s[i - EE_NAME_ADR]);
        }
    }
  EEPROM.end();
}

#ifdef SERIAL_PRINT
  void eePrintName(String prefix, String suffix) {
    EEPROM.begin(EEPROM_LENGTH);
      Serial.print(prefix);
      for (int i = EE_NAME_ADR; i < EE_NAME_ADR + EE_NAME_LEN; i++) {
        Serial.print(char(EEPROM.read(i)));
      }
      Serial.print(suffix);
    EEPROM.end();
  }
#endif

String stringName() {
  String s = "";
  EEPROM.begin(EEPROM_LENGTH);
    for (byte i = EE_NAME_ADR; i < EE_NAME_ADR + EE_NAME_LEN; i++) {
      if (EEPROM.read(i) == 0 || EEPROM.read(i) == 255) {
        break;
      }
      s += char(EEPROM.read(i));
    }
  EEPROM.end();
  return s;
}

// id

void eeClearId() {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = EE_ID_ADR; i < EE_ID_ADR + EE_ID_LEN; i++) {EEPROM.write(i, 0);}
    EEPROM.write(EE_ID_CHECK_ADR, 0);
  EEPROM.end();
}

void eeSetId() {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = EE_ID_ADR; i < EE_ID_ADR + EE_ID_LEN; i++) {
      EEPROM.write(i, buffId[i - EE_ID_ADR]);
    }
    EEPROM.write(EE_ID_CHECK_ADR, EE_ID_CHECK_VAL);
  EEPROM.end();
}

boolean eeCorrectId() {
  byte current, back;
  byte match    = 0;
  byte increase = 0;
  byte decrease = 0;
  EEPROM.begin(EEPROM_LENGTH);
  for (int i = EE_ID_ADR; i < EE_ID_ADR + EE_ID_LEN; i++) {
    current = EEPROM.read(i);
    if (!validChar(current)) {EEPROM.end(); return false;}
    if (current == back)     {match++;    if (match    > 5) {EEPROM.end(); return false;}}
    if (current == back + 1) {increase++; if (increase > 5) {EEPROM.end(); return false;}}
    if (current == back - 1) {decrease++; if (decrease > 5) {EEPROM.end(); return false;}}
    back = current;
  }
  if (EEPROM.read(EE_ID_CHECK_ADR) != EE_ID_CHECK_VAL) {EEPROM.end(); return false;}
  EEPROM.end();
  return true;
}

#ifdef SERIAL_PRINT
  void eePrintId(String prefix, String suffix) {
    EEPROM.begin(EEPROM_LENGTH);
      Serial.print(prefix);
      for (int i = EE_ID_ADR; i < EE_ID_ADR + EE_ID_LEN; i++) {
        Serial.print(char(EEPROM.read(i)));
      }
      Serial.print(suffix);
    EEPROM.end();
  }
#endif

void eeSetBuffId() {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = EE_ID_ADR; i < EE_ID_ADR + EE_ID_LEN; i++) {
      buffId[i - EE_ID_ADR] = EEPROM.read(i);
    }
  EEPROM.end();
}

void eeCheckId() {
  if (!eeCorrectId()) {
    generateId();
    eeSetId();
  } else {
      eeSetBuffId();
    }
}

// EEPROM Info

#ifdef SERIAL_PRINT
  void printEeInfo() {
    eePrintName(F("Name: "), "\n");
    //eePrintId(F("ID: "), "\n"); // seed problem
    //Serial.print(F("Address: "));
    //Serial.print(eeGetAddress()); // seed problem
    //Serial.println();
  }
#endif

/*
void clearEeprom() {
  EEPROM.begin(EEPROM_LENGTH);
    for (int i = 0; i < eeVolume; i++) {
      EEPROM.write(i, 0);
    }
  EEPROM.end();
}

#ifdef SERIAL_PRINT
  void printEeprom(int start, int finish) {
    EEPROM.begin(EEPROM_LENGTH);
      for (int i = start; i < finish; i++) {
        byte val = EEPROM.read(i);
        Serial.print(i);
        Serial.print(F("\t"));
        Serial.print(val);
        Serial.print(F("\t"));
        Serial.print(char(val));
        Serial.println();
      }
    EEPROM.end();
  }
#endif
*/

