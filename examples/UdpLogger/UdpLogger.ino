/**
 * This code shows how you can create and use multiple logger contexts.  
 *
 */


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <UdpLogger.h>
#include <SerialLogger.h>

#include <credentials.h>
/* put the following constants in the ~/Arduino/libraries/Credentials/credentials.h file (thanks for the tip, Andreas Spiess):
#define STASSID "your-access-point-ssid"
#define STAPSK  "your-password"
*/

WiFiUDP Udp;

// IP address and port of the listening log server
// You can set up the server to view the log with:
// nc -u -k -l 8888

// You can even store the log to a file with:
// nc -u -k -l 8888 | tee logFile.log

IPAddress logServerIP(192, 168, 1, 105);
unsigned int logPort = 8888; 

UdpLogger loggerUDP(Udp, logServerIP, logPort, "UDP", LOG_ALL);
SerialLogger loggerSerial(Serial, "SER", LOG_ALL);

void setup() {
  Serial.begin(115200);
  
  loggerSerial.info("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " started.");

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    loggerSerial.info(".");
    delay(500);
  }

  loggerSerial.info("Connected to %s! My IP is address: %s",
    STASSID, WiFi.localIP().toString().c_str());

  Udp.begin(logPort);
  loggerSerial.info("UDP server open on port %d", logPort);

  loggerSerial.info("Connect to the UDP logger with the following command:");
  loggerSerial.info("nc -u %s %d", WiFi.localIP().toString().c_str(), logPort);
  loggerUDP.info("Start now!");
  loggerUDP.info("This log stream is sent to the listening server");
  loggerUDP.info("Please note, UDP is not reliable; some packets may be lost if the network, or any endpoint is loaded");
  loggerUDP.info("The message counter in message prefix will help you detect missing packets.");
}

void loop() {
  // buffer for receiving data
  char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; 

  // Same UDP connexion can receive data
  int packetSize = Udp.parsePacket();
  if (packetSize) 
  {
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;

    loggerSerial.info("Received packet of size %d from %s:%d\t`%s`",
        packetSize,
        Udp.remoteIP().toString().c_str(), Udp.remotePort(),
        packetBuffer);
  }

  static int nextMs = 0;
  const int periodMs = 1000;
  if (millis() > nextMs) 
  {
    nextMs += periodMs;
    loggerUDP.info("UDP logging works at %d", millis());
    loggerSerial.info("Serial logging works at %d", millis());
  }
}
