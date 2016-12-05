/*
  Modul NTP for ESP8266
  part of Arduino Mega Server project
*/

#ifdef NTP_FEATURE

#define TIMEZONE 3
#define NTP_PORT 2390
WiFiUDP udp;

IPAddress timeServerIP; 
const char* ntpServerName = "time.nist.gov";
#define NTP_PACKET_SIZE 48
byte packetBuffer[NTP_PACKET_SIZE]; 
unsigned long ntpTime = 0;

void ntpInit() {
  initStart_("NTP");
  udp.begin(NTP_PORT);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  modulNtp = MODUL_ENABLE;
  initDone_();
}

time_t getNtpTime() {
  getNTP();
  return ntpTime;
}

bool getNTP() {
  WiFi.hostByName(ntpServerName, timeServerIP);
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("No packet yet");
    return false;
  } else {
      Serial.print("packet received, length="); Serial.println(cb);
      udp.read(packetBuffer, NTP_PACKET_SIZE);
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long  lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unsigned long epoch = secsSince1900 - seventyYears;
      ntpTime = epoch + TIMEZONE * 3600;
      Serial.print("Unix time = "); Serial.println(ntpTime);
    }
  return true;
} // getNTP()
 
unsigned long sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0;          // stratum, or type of clock
  packetBuffer[2] = 6;          // polling Interval
  packetBuffer[3] = 0xEC;       // peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

#endif // NTP_FEATURE
