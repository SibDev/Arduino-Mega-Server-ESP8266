/*
  Arduino Mega Server for ESP8266
  version 0.15
  2016, Hi-Lab.ru
  
  License: Free, without any warranties
  Home:    http://hi-lab.ru/arduino-mega-server (russian)
           http://hi-lab.ru/english/arduino-mega-server (english)
  Email:   info@hi-lab.ru
  
  IDE: Arduino 1.6.5 r2
*/

// modules
#define RTC_FEATURE
#define NTP_FEATURE
#define HTTP_FEATURE
//#define FTP_FEATURE
//#define SEND_FEATURE
//#define MAJORDOMO_FEATURE
//#define LAURENT_FEATURE
//#define CONTACTS_FEATURE
//#define PIRS_FEATURE
//#define TEMP_FEATURE
//#define LEDS_FEATURE
//#define KEYS_FEATURE

// debug
//#define ELECTRO_DEBUG
//#define EVENTS_CONTACTS
//#define EVENTS_PIRS
//#define TEMP_DEBUG
#define SERIAL_PRINT

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <FS.h>
#include <Time.h>

char const SELF_NAME[] = "ESP8266";
byte SELF_IP[] = {192, 168, 1, 40};

// Authorization
#define AUTH_OFF 0
#define AUTH_ON  1
byte authMode = AUTH_OFF;
// online encode: base64encode.org
String AUTH_HASH = "Authorization: Basic YWRtaW46YW1z"; // admin:ams

// time provider
#define TIME_NONE    0
#define TIME_NETWORK 1
#define TIME_RTC     2
byte timeProvider = TIME_NETWORK;

// modules
#define MODUL_DISABLE       0
#define MODUL_ENABLE        1
#define MODUL_NOT_COMPILLED 2
byte modulRtc      = MODUL_NOT_COMPILLED;
byte modulNtp      = MODUL_NOT_COMPILLED;
byte modulSend     = MODUL_NOT_COMPILLED;
byte modulMajor    = MODUL_NOT_COMPILLED;
byte modulLaurent  = MODUL_NOT_COMPILLED;
byte modulPirs     = MODUL_NOT_COMPILLED;
byte modulContacts = MODUL_NOT_COMPILLED;
byte modulTemp     = MODUL_NOT_COMPILLED;
byte modulLeds     = MODUL_NOT_COMPILLED; 
byte modulKeys     = MODUL_NOT_COMPILLED;
byte modulFtp      = MODUL_NOT_COMPILLED;

byte modulEthernet = MODUL_NOT_COMPILLED;
byte modulSd       = MODUL_NOT_COMPILLED;
byte modulServer   = MODUL_NOT_COMPILLED;
byte modulPing     = MODUL_NOT_COMPILLED;
byte modulUpload   = MODUL_NOT_COMPILLED;
byte modulElectro  = MODUL_NOT_COMPILLED;
byte modulNoo      = MODUL_NOT_COMPILLED;
byte modulMr1132   = MODUL_NOT_COMPILLED;
byte modulNrf24    = MODUL_NOT_COMPILLED;

// timers
boolean cycle1s  = false;
boolean cycle4s  = false;
boolean cycle20s = false;
boolean cycle30s = false;
boolean cycle1m  = false;
boolean cycle3m  = false;
boolean cycle5m  = false;

// strings
char buf[200];

// for sample Ajax
boolean LED_state[4] = {0};

#ifdef LEDS_FEATURE
  // LED modes
  #define LED_EMPTY 0
  #define LED_PIR 1
  byte modeLed = LED_EMPTY;
#endif

/* setup
------------------------------------- */

void setup() {
  Serial.begin(115200);
  Serial.print(F("\nAMS for "));
  Serial.print(SELF_NAME);
  Serial.println(" started...");
  hardwareInit();
  timersInit();
  wifiInit();
  spiffsInit();
  randomInit();
  eepromInit();
  ntpInit();
  rtcInit();
  #ifdef HTTP_FEATURE
    httpInit();
  #endif
  #ifdef FTP_FEATURE
    ftpInit();
  #endif
  serverInit();
  #ifdef MAJORDOMO_FEATURE
    majordomoInit();
  #endif
  #ifdef LAURENT_FEATURE
    laurentInit();
  #endif
  #ifdef TEMP_FEATURE
    tempInit();
  #endif
  #ifdef CONTACTS_FEATURE
    contactsInit();
  #endif
  #ifdef PIRS_FEATURE
    pirsInit();
  #endif
  #ifdef LEDS_FEATURE
    ledsInit();
  #endif
  #ifdef KEYS_FEATURE
    keysInit();
  #endif
  Serial.print(F("GLOBAL Init DONE ("));
  Serial.print(millis() / 1000);
  Serial.println(F("s)\n"));
  Serial.println(F("AMS WORK"));
  timeStamp(); printFreeMem("");
} // setup

/* setup
------------------------------------- */

void loop() {
  timersWorks();
  rtcWorks();
  //clockWork();
  
  #ifdef HTTP_FEATURE
    httpWork();
  #endif
  #ifdef FTP_FEATURE
    ftpWork();
  #endif
  
  serverWorks();
  
  #ifdef TEMP_FEATURE
    if (cycle5m) {tempWorks();}
  #endif
  #ifdef CONTACTS_FEATURE
    contactsWorks();
  #endif
  #ifdef PIRS_FEATURE
    pirsWorks();
  #endif
  #ifdef KEYS_FEATURE
    keysWorks();
  #endif
  #ifdef LAURENT_FEATURE
    if (cycle5m) {
      sprintf(buf, "$KE");
      sendLaurentRequest();
    }
  #endif

  if (cycle20s) {
    //
  }

  if (cycle1m) {
    timeStamp(); printFreeMem("");
  }

  // end loop works
  cyclosInSecWork();
  cyclosDelayWork();
  eraseCyclos();
} // loop

