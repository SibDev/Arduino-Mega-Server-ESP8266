/*
  Modul Page Maker for ESP8266
  part of Arduino Mega Server project
*/

#define OPEN_BLOCK_STYLE "\n<style>\n"
#define CLOSE_BLOCK_STYLE "</style>\n"
#define CLOSE_STYLE "  }\n"

#define HTM_EXT ".htm"
File insertFile;

String openStyle(String style) {
  String s = "  .";
  s += style;
  s += " {\n";
  return s;
}

// <style>
//   .style {
//     background: url(//ip/fileName) no-repeat;
//   }
// </style>"

String backgroundIpAttr(String fileName) {
  String s = "    ";
  s += "background: url(//";
  s += ipString(SELF_IP);
  s += "/";
  s += fileName;
  s += ") no-repeat;\n";
  return s;
}

void addBackgroundStyle(String style, String file, WiFiClient cl) {
  String s = OPEN_BLOCK_STYLE;
  s += openStyle(style);
  s += backgroundIpAttr(file);
  s += CLOSE_STYLE;
  s += CLOSE_BLOCK_STYLE;
  cl.println(s);
}

String themeSuffix(byte design) {
  switch (design) {
    case HOME_DESIGN:   return "_hm"; break;
    case MODERN_DESIGN: return "_md"; break;
    case HACKER_DESIGN: return "_hk"; break;
    case PART1_DESIGN:  return "_p1"; break;
    case PART2_DESIGN:  return "_p2"; break;
    case PART3_DESIGN:  return "_p3"; break;
    case PART4_DESIGN:  return "_p4"; break;
               default: return "";
  }
}

void insertBlock(uint8_t operation, WiFiClient cl) {
  String s = "";
  switch (operation) {
    case LINKS:
      s = themeSuffix(currentDesign);
      if (s == "_hm") {addBackgroundStyle("home",   "home.jpg",   cl);}
      if (s == "_md") {addBackgroundStyle("modern", "modern.jpg", cl);}
      insertFile = SPIFFS.open("/_one" + s + HTM_EXT, "r");
      break;
    case HEADER:     s = themeSuffix(currentDesign); if (s == "") {s = "er";} insertFile = SPIFFS.open("/_head" + s + HTM_EXT, "r"); break;
    case FOOTER:     s = themeSuffix(currentDesign); if (s == "") {s = "er";} insertFile = SPIFFS.open("/_foot" + s + HTM_EXT, "r"); break;
    case BANNERS:    if (random(1, 15) == 5) {insertFile = SPIFFS.open("/_banner.htm", "r");} break;
    case DASH:       insertFile = SPIFFS.open("/_dash" + themeSuffix(currentDesign) + HTM_EXT, "r"); break;
    case MENU:       insertFile = SPIFFS.open("/_menu" + themeSuffix(currentDesign) + HTM_EXT, "r"); break;
    case ADDRESS:    cl.print(ipString(SELF_IP)); break;
    case SCRIPTS:    insertFile = SPIFFS.open("/scripts.js", "r"); break;
    case FLOTR2:     insertFile = SPIFFS.open("/flotr2.js",  "r"); break;
    case PROCESSING: insertFile = SPIFFS.open("/process.js", "r"); break;
    case THREE:      insertFile = SPIFFS.open("/three.js",   "r"); break;
    case JQUERY:     insertFile = SPIFFS.open("/jquery.js",  "r"); break;
    default: 
      {}
  } // switch

  if (insertFile) {
    cl.write(insertFile, HTTP_DOWNLOAD_UNIT_SIZE_2);
    insertFile.close();
  }
} // insertBlock( )

