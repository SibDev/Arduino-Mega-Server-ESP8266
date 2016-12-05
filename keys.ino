/*
  Modul Keys for ESP8266
  part of Arduino Mega Server project
*/

#ifdef KEYS_FEATURE

#define KEY_PIN 14

int goLight = 0; // key (light) control

void keysInit() {
  pinMode(KEY_PIN, OUTPUT);
  modulKeys = MODUL_ENABLE;
  started("Keys");
}

void keysWorks() {
  if (goLight == 1) {
    digitalWrite(KEY_PIN, LOW);
  } else {
      digitalWrite(KEY_PIN, HIGH);   
    }
}

#endif // KEYS_FEATURE
