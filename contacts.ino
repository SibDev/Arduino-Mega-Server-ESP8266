/*
  Modul Contacts for ESP8266
  part of Arduino Mega Server project
*/

#ifdef CONTACTS_FEATURE

#define CONT_1_PIN 12
#define CONT_2_PIN 13

#define STATE_NOT_DEFINED 2
#define OPEN  1
#define CLOSE 0

#define CONT_1_OBJECT_NAME "Door1"
#define CONT_2_OBJECT_NAME "Contact2"

#define OPEN_STR "Open"
#define CLOSED_STR "Closed"
#define CONT_2_OPEN_MESS "Down"
#define CONT_2_CLOSE_MESS "Norm"

// states
byte cont1OldState = STATE_NOT_DEFINED; byte cont1ObjState = STATE_NOT_DEFINED;
byte cont2OldState = STATE_NOT_DEFINED; byte cont2ObjState = STATE_NOT_DEFINED;

void contactsInit() {
  pinMode(CONT_1_PIN, INPUT);
  pinMode(CONT_2_PIN, INPUT);
  modulContacts = MODUL_ENABLE;
  started("Contacts");
}

void printContactState(char obj[], char mess[]) {
  #ifdef EVENTS_CONTACTS
    timeStamp();
    Serial.print(obj);
    Serial.print(": ");
    Serial.println(mess);
  #endif
}

void contact1Actions(byte state, char mess[], byte major) {
  cont1ObjState = state;
  printContactState(CONT_1_OBJECT_NAME, mess);
  #ifdef MAJORDOMO_FEATURE
    sendRequestM(CONT_1_OBJECT_NAME, major);
  #endif
}

void contact2Actions(byte state, char mess[], byte major) {
  cont2ObjState = state;
  printContactState(CONT_2_OBJECT_NAME, mess);
  #ifdef MAJORDOMO_FEATURE
    sendRequestM(CONT_2_OBJECT_NAME, major);
  #endif
}

void contact1Work() {
  byte cont1State = digitalRead(CONT_1_PIN);
  if (cont1State != cont1OldState) {
    if (cont1State == HIGH) {contact1Actions(OPEN,  OPEN_STR,   OPEN);}
                       else {contact1Actions(CLOSE, CLOSED_STR, CLOSE);}
    cont1OldState = cont1State;
  }
}

void contact2Work() {
  byte cont2State = digitalRead(CONT_2_PIN);
  if (cont2State != cont2OldState) {
    if (cont2State == LOW) {contact2Actions(OPEN,  CONT_2_OPEN_MESS,  OPEN);}
                      else {contact2Actions(CLOSE, CONT_2_CLOSE_MESS, CLOSE);}
    cont2OldState = cont2State;
  }
}

void contactsWorks() {
  contact1Work();
  contact2Work();
}

#endif // CONTACTS_FEATURE
