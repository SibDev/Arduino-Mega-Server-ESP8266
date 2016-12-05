# Arduino Mega Server for ESP8266
version 0.15
2016, Hi-Lab.ru

License: Free, without any warranties
Home:    http://hi-lab.ru/arduino-mega-server (russian)
		 http://hi-lab.ru/english/arduino-mega-server (english)
Email:   info@hi-lab.ru

IDE: Arduino 1.6.5 r2

Hardware:
---------
ESP-12-F QIO L4 (or similar)

Pathes of project:
------------------
\Sketches\esp8266\Arduino\arduino_mega_server_esp8266\
\Sketches\esp8266\Arduino\libraries\

Arduino IDE settings:
---------------------
Sketches folder:
\Sketches\esp8266\Arduino

Quick start:
------------
0. Connect ESP8266 module to USB port use Arduino Uno (or USB-UART adapter)

1. Install Arduino IDE 1.6.5 (r2) or high
2. Install ESP8266 drivers to Arduino IDE
3. Install ESP8266 Sketch Data Upload (ESP8266FS-0.1.3.zip) to folder /Arduino/tools/

4. Make SPIFFS filesystem on module ESP8266 4M (3M SPIFFS) with utility ESP8266 Sketch Data Upload
5. Upload Sketch "arduino_mega_server_esp8266" to module ESP8266

6. Print SSID and PASSWORD of your Wi-Fi router in module "Wi-Fi" of sketch
7. Open in your browser address "192.168.1.40"
8. Enjoy and donate on page http://hi-lab.ru/arduino-mega-server/details/donate
