/* ----------------------------------------------
  Modul Send HTTP requests for ESP8266
  part of Arduino Mega Server project
------------------------------------------------- */

#ifdef SEND_FEATURE

// target net settings
byte TARGET_IP[] = {192, 168, 1, 8};
int TARGET_PORT = 80;

WiFiClient tclient;

void sendInit() {
  initStart("Send");
  modulSend = MODUL_ENABLE;
  initDone();
}

void sendHttpRequest(byte ip[], int port, WiFiClient cl) {
  String s = "";
  if (cl.connect(ip, port)) { 
    timeStamp();
    Serial.print("Host ");
    printIp(ip);
    Serial.print(" ");
    Serial.println(buf);
    
    s += String(buf);
    s += '\n';
    s += "Host: ";
    s += ipString(SELF_IP);
    s += '\n';
    cl.println(s); 
    delay(100);
    cl.stop();
  } else {
      timeStamp();
      Serial.print("Host ");
      printIp(ip);
      Serial.print(" not connected (");
      Serial.print(buf);
      Serial.println(")");
    }
}

void sendRequest(byte ip[], int port, char object[], int value, WiFiClient cl) {
  sprintf(buf, "GET /?%s=%d", object, value); 
  sendHttpRequest(ip, port, cl);
}

#endif // SEND_FEATURE
