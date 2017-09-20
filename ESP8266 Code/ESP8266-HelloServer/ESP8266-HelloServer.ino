/*
   Build details
   =============

   Source: https://github.com/ianbren/Fibaro_Esp8266_101
   Based on ESP8266 example HELLOSERVER with unrequired code commented out

   modified by: ianbren at hotmail dot com

   Used as sample application to show simple integration with FIBARO home automation system
   Return type has been changed to "text" to enable simple debug in browser

   Tested on WEMOS D1 mini - no hardware dependencies expected

   TODO: Update WIFI details before the build

*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// ***** Add Your Wifi Details here before Build
const char* ssid = "........";
const char* password = "........";

// Some JSON
const char* jsonData = "{\"Data1\": \"1\",\"Data2\": \"2\", \"myText\": \"Hello World\"}";

// Default Web Port is 80
ESP8266WebServer server(80);

const int led = LED_BUILTIN;

void handleRoot() {
  Serial.println("In HandleRoot");
  digitalWrite(led, 1);
  // type changed to "text/plain" so it's easy to see in any type of browser
  server.send(200, "text/plain", jsonData);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //  if (MDNS.begin("fibaroEsp8266")) {
  //    Serial.println("MDNS responder started");
  //  }
  //
  server.on("/", handleRoot);

  //  server.on("/inline", []() {
  //    server.send(200, "text/plain", "this works as well");
  //  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
