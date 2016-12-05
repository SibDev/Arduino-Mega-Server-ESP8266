/*
  Modul Server Commands for ESP8266
  part of Arduino Mega Server project

  Key ON:  rele4=1
  Key OFF: rele4=.

  Night mode ON:  night=1
  Night mode OFF: night=.

  Color LEDS: color=white, black, red, blue, green, yellow, magenta, cyan
*/

#ifdef SERVER_FEATURE

String command;
String parameter;

void setKey() {
  if (command.indexOf("rele4") >= 0) {
    if (parameter.indexOf("1") >= 0) {
      //setGoLightON();
    } else {
        //setGoLightOFF();
      }
  }
}

#ifdef LEDS_FEATURE
  void setNightLed() {
    if (command.indexOf("night") >= 0) {
      if (parameter.indexOf("1") >= 0) {
        modeLED = LED_EMPTY;
      } else {
           modeLED = LED_PIR;
        }
    }
  }

  void setColorLed() {
    if (command.indexOf("color") >= 0) {
      if (parameter.indexOf("black")   >= 0) {black   (led1);}
      if (parameter.indexOf("white")   >= 0) {white   (led1, 255);}
      if (parameter.indexOf("red")     >= 0) {red     (led1, 255);}
      if (parameter.indexOf("blue")    >= 0) {blue    (led1, 255);}
      if (parameter.indexOf("green")   >= 0) {green   (led1, 255);}                            
      if (parameter.indexOf("yellow")  >= 0) {yellow  (led1, 255);}                            
      if (parameter.indexOf("magenta") >= 0) {magenta (led1, 255);}                            
      if (parameter.indexOf("cyan")    >= 0) {cyan    (led1, 255);}
    }
  }
#endif // LEDS_FEATURE

/* ----------------------------------------
  Function parseCommands(EthernetClient cl)
    Parse network commands
------------------------------------------- */

void parseCommands(WiFiClient cl) {
  int posBegin;
  int posEnd;
  int posParam;
  
  if (request.indexOf("?") >= 0) {
    posBegin = request.indexOf("?") + 1;
    posEnd = request.indexOf("HTTP");

    if (request.indexOf("=") >= 0) {
       posParam = request.indexOf("=");
       command = request.substring(posBegin, posParam);              
       parameter = request.substring(posParam + 1, posEnd - 1);              
    } else {
        command = request.substring(posBegin, posEnd - 1);              
        parameter = "";
      }

    Serialprint("command: "); Serial.println(command);
    Serialprint("parameter: "); Serial.println(parameter);
          
    setKey();
    
    #ifdef LEDS_FEATURE
      setNightLed();
      setColorLed();
    #endif
    
    // erase request
    request = "";
  } // if (url.indexOf("?") >= 0)
} // parseCommands

#endif // SERVER_FEATURE
