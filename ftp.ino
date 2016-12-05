/*
  Modul FTP for ESP8266
  part of Arduino Mega Server project
*/

#ifdef FTP_FEATURE

#include <ESP8266FtpServer.h>

#define FTP_LOGIN "admin"
#define FTP_PASSWORD "ams"

FtpServer ftpSrv;

void ftpInit() {
  initStart("FTP");
  ftpSrv.begin(FTP_LOGIN, FTP_PASSWORD);
  timeStamp();
  printValue(LOGIN_STR, FTP_LOGIN);
  timeStamp();
  printValue(PASSWORD_STR, FTP_PASSWORD);
  modulFtp = MODUL_ENABLE;
  initDone();
}

void ftpWork() {
  ftpSrv.handleFTP();
}

#endif // FTP_FEATURE
