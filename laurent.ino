/*
  Modul Laurent for ESP8266
  part of Arduino Mega Server project
 
  byte CRbyte = 13; // HEX 0x0D
  byte LFbyte = 10; // HEX 0x0A
*/

#ifdef LAURENT_FEATURE

byte LAURENT_IP[] = {192, 168, 1, 19};
#define LAURENT_PORT 2424

#define MAX_LEN_LREQUEST 25
String lrequest = "";

WiFiClient lclient;

// init

void laurentInit() {
  initStart("Laurent");
  timeStamp();
  Serial.print("Connect to Laurent... ");
  if (lclient.connect(LAURENT_IP, LAURENT_PORT)) {
    Serial.println("OK");
    lclient.stop();
    
    // Send test command
    Serial.println("Selftest...");
    sprintf(buf, "$KE");   
    sendLaurentRequest();

    // Send password (default: "Laurent")
    Serial.println("Set password...");
    sprintf(buf, "$KE,PSW,SET,Laurent");   
    sendLaurentRequest();
  } else {
      Serial.println("failed");
    }
  delay(500);
    
  // stop sending DATA
  sprintf(buf, "$KE,DAT,OFF");
  sendLaurentRequest();
  delay(100);
  
  // rele 2 OFF
  sprintf(buf, "$KE,REL,2,0");
  sendLaurentRequest();
  
  modulLaurent = MODUL_ENABLE;
  initDone();
} // laurentInit()

void sendLaurentRequest() {
  if (lclient.connect(LAURENT_IP, LAURENT_PORT)) {
    timeStamp();
    Serial.print("Command: ");
    Serial.println(buf);
    lclient.println(buf);
    delay(100);
    timeStamp();
    Serial.print("Answer:  ");
    lrequest = "";
    while(lclient.available() != 0) {
      char c = lclient.read();
      Serial.print(c);
      if (lrequest.length() < MAX_LEN_LREQUEST) {
        lrequest += c;
      }
    }
    delay(300);
    lclient.stop();
  } else {
      timeStamp();
      Serial.println("Error sending command");
    }
} // sendLaurentRequest()

#endif // LAURENT_FEATURE
