/* ----------------------------------------------
  Modul MajorDoMo for ESP8266
  part of Arduino Mega Server project
  
  sprintf(buf, "GET /objects/?script=testScript");
  sendHTTPRequest();
------------------------------------------------- */

#ifdef MAJORDOMO_FEATURE

char majorMegaObject[] = "ncMega01";
char majorCommObject[] = "comm_mega01";

// MajorDoMo net settings
byte MAJOR_IP[] = {192, 168, 2, 8};
int MAJOR_PORT = 80;

// EthernetClient object
WiFiClient mclient;

void majordomoInit() {
  initStart("MajorDoMo");
  timeStamp();
  Serial.print("Connect to MajorDoMo... ");
  if (mclient.connect(MAJOR_IP, MAJOR_PORT)) {
    Serial.println("OK");
    //mclient.println("GET /search?q=arduino HTTP/1.0");
    mclient.println();
  } else {
      Serial.println("failed");
    }
  delay(200);
  mclient.stop();
  modulMajor = MODUL_ENABLE;
  initDone();
}
/*void checkMajor() {
  Serial.print("Connect to MajorDoMo... ");
  if (mclient.connect(MAJOR_IP, MAJOR_PORT)) {
    stationMajor = STATION_PRESENT;
    Serial.println("OK");
    //mclient.println("GET /search?q=arduino HTTP/1.0");
    mclient.println();
  } else {
      stationMajor = STATION_DOWN;
      Serial.println("failed");
    }
  delay(200);
  mclient.stop();
}
*/
/*
void sendMajorRequest() {
  if (mclient.connect(MAJOR_IP, MAJOR_PORT)) { 
    timeStamp();
    Serial.println(buf);
    mclient.println(buf);
    mclient.println("Host: 192.168.2.36");
    mclient.println();   
    delay(100); // 500 max
    mclient.stop();
  } else {
      timeStamp();
      Serial.print("MajorDoMo not connected (");
      Serial.print(buf);
      Serial.println(")");
    }
}
*/

/* ---------------------
  Function sendRequest
    object:   <...>
    method:   "update"
    variable: "v"
    value:    <...>
------------------------ */

void sendRequestM(char object[], int value) {
  sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%d", object, value); 
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
}

/* --------------------------------------------
  Function sendRequestMinus
    Посылка объект - значение с дробной частью
    и подстановкой знака перед значением
    object:   <...>
    method:   "update"
    variable: "v"
    value:    minus<...>.<...>
----------------------------------------------- */

void sendRequestMinusM(char object[], char minus[], int value, int value_) {
  sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%s%0d.%d HTTP/1.0", object, minus, value, value_);
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
}

/* -----------------------------
  Function sendRequestVariable ?
    object:   <...>
    method:   "update"
    variable: <...>
    value:    <...>
-------------------------------- */

void sendRequestVariableM(char object[], char variable[], int value) {
  sprintf(buf, "GET /objects/?object=%s&op=m&m=update&%s=%d", object, variable, value); 
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
}

/* -------------------------------------------
  Function sendReqSensor ?
    Посылка объект - значение с дробной частью
    (метод и переменная заранее заданы)
    object:   ...
    method:   "update"
    variable: "v"
    value:    "..."."..."
---------------------------------------------- */

void sendReqSensorM(char object[], int value, int value_) {
  sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%0d.%d HTTP/1.0", object, value, value_);
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
}

/* ----------------------------------------------------
  Function sendReqSensor_ ?
    Посылка объект - значение с дробной частью
    с условием если дробная часть < 10, добавляется s2,
    если > 10, то добавляется s3    
    object:   ...
    method:   "update"
    variable: "v"
    value:    "...".zero"..."
------------------------------------------------------ */

void sendReqSensorM_(char object[], char zero[], char empty[], int value, int value_) {
  if (value_ < 10) {
    sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%0d.%s%d HTTP/1.0", object, value, zero, value_);
  } else {
    sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%0d.%s%d HTTP/1.0", object, value, empty, value_);
  }
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
}

/* ---------------------------------------------------------
  Function sendReqSensorFloat ?
    Стандартная посылка объект - значение с плавающей точкой
    (метод и переменная заранее заданы)
    object:   ...
    method:   "update"
    variable: "v"
    value:    ...
------------------------------------------------------------ */

void sendReqSensorFloatM(char object[], float value) {
  sprintf(buf, "GET /objects/?object=%s&op=m&m=update&v=%F HTTP/1.0", object, value);
  //sendMajorRequest();
  sendHttpRequest(MAJOR_IP, MAJOR_PORT, mclient);
} 

/* ------------------------------------------------------
  Function majordomoMegaLive
    Уведомление MajorDomo о том, что Mega жива и работает
--------------------------------------------------------- */

void majordomoMegaLive() {
  if (cycle1m) {
    sendRequestM(majorMegaObject, 1);
    //checkEvent(&prevEventMegaLive);
  }
}

#endif // MAJORDOMO_FEATURE
