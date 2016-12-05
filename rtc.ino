/*
  Modul RTC for ESP8266
  part of Arduino Mega Server project
*/

void rtcInit() {
  initStart_("RTC");
  Serial.println(F("Set time provider"));
  setSyncProvider(getNtpTime);
  modulRtc = MODUL_ENABLE;
  initDone_();
}

void displayTime() {
  uint16_t m = (now() / 60) % 60;
  uint16_t h = (now() / 3600) % 24;
  if (h < 10) {Serial.print("0");}
  Serial.print(h);
  Serial.print(":");
  if (m < 10) {Serial.print("0");}
  Serial.print(m);
}

void printDigits(int digits) {
  if (digits < 10) {Serial.print('0');}
  Serial.print(digits);
}

void timeStamp() {
  Serial.print(year()); 
  Serial.print("-");
  printDigits(month());
  Serial.print("-");
  printDigits(day());
  Serial.print(" ");
  printDigits(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  Serial.print(" ");
}

void printRTC(){
  timeStamp();
  Serial.println();
}

String uptimeString() {
  char uptime[14];
  char zero[] = "0";
  char nop[]  = "";
  char* zh;
  char* zm;
  char* zs;

  unsigned long totalSeconds = millis() / 1000;
  unsigned long days = totalSeconds / 86400;
  unsigned long tsHours = totalSeconds - days * 86400;
  unsigned long hours = tsHours / 3600;
  unsigned long tsMinutes = tsHours - hours * 3600;
  unsigned long minutes = tsMinutes / 60;
  unsigned long seconds = tsMinutes - minutes * 60;

  if (hours   < 10) {zh = zero;} else {zh = nop;}
  if (minutes < 10) {zm = zero;} else {zm = nop;}
  if (seconds < 10) {zs = zero;} else {zs = nop;}

  sprintf(uptime, "%d %s%d:%s%d:%s%d", (int)days, zh, (int)hours, zm, (int)minutes, zs, (int)seconds);
  return String(uptime);
} // uptimeString()

// Lifer

byte lifer;

void setLifer() {
  lifer++;
  if (lifer > 6) {
    lifer = 0;
  }
}

#ifdef NTP_FEATURE
  void setNtpInterval() {
    if (timeStatus() == timeNotSet) {
      setSyncInterval(20);
    } else {
        setSyncInterval(3600);
      }
  }
#endif
  
// rtcWorks

void rtcWorks() {
  #ifdef NTP_FEATURE
    if (cycle20s) {setNtpInterval();}
  #endif
  if (cycle1s) {setLifer();}
}

