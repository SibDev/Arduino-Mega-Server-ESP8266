/*
  Modul Server for ESP8266
  part of Arduino Mega Server project
*/

#define HTTP_DOWNLOAD_UNIT_SIZE_2 2920
#define SERVER_PORT 80

WiFiServer server(SERVER_PORT);

// HTTP request
#define REQ_BUF_SIZE 128
char HTTP_req[REQ_BUF_SIZE] = {0}; // null terminated string
int reqIndex = 0;

// string request
#define MAX_LEN_REQUEST 512
String request = "";

// webFile
File webFile;
#define MAX_BUFFER_SIZE 2912
uint16_t rsize;
uint8_t buff[MAX_BUFFER_SIZE];

// markers
boolean allowMarkers = false;
#define MARKER     0x25 // %
#define LINKS      0x26 // &
#define HEADER     0x31 // 1
#define DASH       0x32 // 2
#define MENU       0x33 // 3
#define FOOTER     0x34 // 4
#define BANNERS    0x35 // 5
#define ADDRESS    0x21 // !
#define SCRIPTS    0x23 // #
#define FLOTR2     0x3F // ?
#define PROCESSING 0x2B // +
#define JQUERY     0x3D // =
#define THREE      0x40 // @

void serverInit() {
  server.begin();
  modulServer = MODUL_ENABLE;
  started("Server");
}

int checkMarker(int startPos, int endPos) {
  for (int i = startPos; i < endPos; i++) {
    if (buff[i] == MARKER && (buff[i + 1] == HEADER ||
                              buff[i + 1] == LINKS ||
                              buff[i + 1] == DASH ||
                              buff[i + 1] == MENU ||
                              buff[i + 1] == FOOTER ||
                              buff[i + 1] == BANNERS ||
                              buff[i + 1] == ADDRESS ||
                              buff[i + 1] == SCRIPTS ||
                              buff[i + 1] == FLOTR2 ||
                              buff[i + 1] == PROCESSING ||
                              buff[i + 1] == JQUERY ||
                              buff[i + 1] == THREE)) {
      return i;
    }
  }
  return -1;
}

int sendDelta(int startPos, int endPos, WiFiClient cl) {
  String s = "";
  for (int i = startPos; i < endPos; i++) {s += char(buff[i]);}
  cl.print(s);
}

void markersWorks(int pos, WiFiClient cl) {
  if (pos > rsize - 2) {return;}
  int markerPos = checkMarker(pos, rsize - 1);
  if (markerPos > -1) {
    sendDelta(pos, markerPos, cl);
    insertBlock(buff[markerPos + 1], cl);
    markersWorks(markerPos + 2, cl);
  } else {
      if (pos == 0) {
        String s = "";
        for (int i = 0; i < rsize; i++) {s += char(buff[i]);}
        cl.print(s);
      } else {
          sendDelta(pos, rsize, cl);
        }
    }
}

void StrClear(char *str, char length) {
  for (int i = 0; i < length; i++) {
    str[i] = 0;
  }
}

char StrContains(char *str, char *sfind) {
  char found = 0;
  char index = 0;
  char len;

  len = strlen(str);
  if (strlen(sfind) > len) {return 0;}
  
  while (index < len) {
    if (str[index] == sfind[found]) {
      found++;
      if (strlen(sfind) == found) {return 1;}
    } else {
        found = 0;
      }
    index++;
  }
  return 0;
}

int curMem = 0;
int oldMem = 0;

void printFreeMem(String s) {
  Serial.print(s);
  Serial.print(": ");
  curMem = ESP.getFreeHeap();
  Serial.print(curMem);
  Serial.print(" (");
  byte perc = curMem / PERC_FREE_MEM;
  Serial.print(perc);
  Serial.print("%) ");
  Serial.println(curMem - oldMem);
  oldMem = curMem;
}

void serverWorks() {
  WiFiClient wclient = server.available();
  if (wclient) {
    boolean currentLineIsBlank = true;
    
    while (wclient.connected()) {
      if (wclient.available()) {
        char c = wclient.read();

        // leave last element as 0 to null terminate string
        if (reqIndex < REQ_BUF_SIZE - 1) {
          HTTP_req[reqIndex] = c;
          reqIndex++;
        }
        
        if (request.length() < MAX_LEN_REQUEST) {
          request += c;
        }
        
        if (c == '\n' && currentLineIsBlank) {
          if (authMode == AUTH_OFF || (authMode == AUTH_ON && request.lastIndexOf(AUTH_HASH) > -1)) {
            //parseCommands(wclient);
            parseRequest(wclient); 

            if (webFile) {
              if (allowMarkers) {
                while(webFile.available()) {
                  rsize = webFile.read(buff, MAX_BUFFER_SIZE);
                  markersWorks(0, wclient);
                }
              } else {
                  wclient.write(webFile, HTTP_DOWNLOAD_UNIT_SIZE_2);
                }
              webFile.close();
            } // if (webFile)
          
            // Reset
            reqIndex = 0;
            StrClear(HTTP_req, REQ_BUF_SIZE);
            request = "";
          } else { // if (authMode == AUTH_OFF || (authMode == AUTH_ON && request.lastIndexOf(AUTH_HASH) > -1))
              wclient.println("HTTP/1.0 401 Unauthorized\nWWW-Authenticate: Basic realm=\"Arduino Mega Server\"");
              request = "";
            }
            break;

        } // if (c == '\n' && currentLineIsBlank)
        
        // every received line end with \r\n
        if (c == '\n') {
          currentLineIsBlank = true; // last character on line starting new line with next character read
        }
        else if (c != '\r') {
          currentLineIsBlank = false; // text character was received
        }
      } // if (wclient.available())
    } // while (wclient.connected())
    delay(10); // time to browser receive data

    wclient.stop(); // close connection
  } // if (wclient)
} // serverWorks()

