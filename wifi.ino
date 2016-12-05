/*
  Modul Wi-Fi for ESP8266
  part of Arduino Mega Server project
*/

char ssid[] = "ssid";
char pass[] = "password";

IPAddress ip = SELF_IP;
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void wifiInit() {
  initStart_("Wi-Fi");
  Serial.print("Connecting to "); Serial.println(ssid);
  WiFi.begin(ssid, pass);
  WiFi.config(ip, gateway, subnet);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  initDone_();
}

