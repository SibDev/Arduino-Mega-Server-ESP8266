/*
  Modul Common for ESP8266
  part of Arduino Mega Server project
*/

#define LOGIN_STR "Login"
#define PASSWORD_STR "Password"

// init

void started(String s) {
  timeStamp();
  Serial.print("Modul ");
  Serial.print(s);
  Serial.println("... started");
}

void started_(String s) {
  Serial.print("Modul ");
  Serial.print(s);
  Serial.println("... started");
}

void initStart(String s) {
  timeStamp();
  Serial.print("Modul ");
  Serial.print(s);
  Serial.println("...");
}

void initStart_(String s) {
  Serial.print("Modul ");
  Serial.print(s);
  Serial.println("...");
}

void initDone() {
  timeStamp();
  Serial.println(F("started"));
}

void initDone_() {
  Serial.println(F("started"));
}

// print values

void printValue(String parameter, String value) {
  Serial.print(parameter);
  Serial.print(": ");
  Serial.println(value);
}

// print network

void printIp(byte ip[]) {
  for (byte i = 0; i < 4; i++) {
    Serial.print(ip[i]);
    if (i == 3) {break;}
    Serial.print(".");
  }
}

void printMac(byte mac[]) {
  for (byte i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i == 5) {break;}
    Serial.print(" ");
  }
}

// string network

String ipString(byte ip[]) {
  String s = "";
  for (byte i = 0; i < 4; i++) {
    s += ip[i];
    if (i == 3) {return s;}
    s += '.';
  }
}

