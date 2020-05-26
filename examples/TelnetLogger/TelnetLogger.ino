/**
 * This code shows how you can create and use the Telnet logger
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <SerialLogger.h>
#include <TelnetLogger.h>

#include <credentials.h>
/* put the following constants in the ~/Arduino/libraries/Credentials/credentials.h file (thanks for the tip, Andreas Spiess):
#define STASSID "your-access-point-ssid"
#define STAPSK  "your-password"
*/

SerialLogger loggerSerial(Serial, "SER", LOG_ALL);
TelnetLogger loggerTelnet(/* port */23, /* context */"TelnetDemo", /* level */LOG_ALL);
/*
TelnetLogger loggerTelnet(); // Default constructor uses port 23, context "DFT", level LOG_ALL.
*/


void setup() {
  Serial.begin(115200);
  delay(1000);

  loggerSerial.info("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " started.");

  // Connect Wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    loggerSerial.info(".");
    delay(500);
  }

  loggerSerial.info("I'm connected to %s. You can now connect with `$ telnet %s 23`",
    STASSID, WiFi.localIP().toString().c_str());
}

void loop() {
  static int nextMs = 0;
  const int periodMs = 1000;
  if (millis() >= nextMs)
  {
    nextMs += periodMs;

    loggerSerial.info("Serial logging works at %d, Telnet logger client is%s connected",
        millis(), loggerTelnet.isClientConnected() ? "" : " not");

    loggerTelnet.info("Telnet logging from %s, still up at %d",
        WiFi.localIP().toString().c_str(), millis());

  }
}
