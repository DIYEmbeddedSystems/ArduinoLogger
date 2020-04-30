/**
 * This code shows how you can stream logs to multiple backends, e.g. both Serial and UDP. 
 */


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <UdpLogger.h>
#include <SerialLogger.h>
#include <DupLogger.h>


#define STASSID "your-AP"
#define STAPSK  "your-password"


// Create a UDP logger
WiFiUDP Udp;
// IP address and port of the listening log server
// You can set up the server to view the log with:
// nc -u -k -l 8888
UdpLogger loggerUDP(Udp, IPAddress(192, 168, 1, 105), 8888, "UDP", LOG_ALL);


// Create a duplicate logger, with UDP logger and default logger as backends
DupLogger logger(Logger::getDefault(), loggerUDP);

void setup() {
  Serial.begin(115200);
  static char espName[128];
  snprintf(espName, sizeof(espName), "ESP-%08x",  ESP.getChipId());
  
  logger.info("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " started.");

  INFO("\nApplication " __FILE__ " compiled " __DATE__ " at " __TIME__ "\n\n");
  INFO("Reset reason: %s", ESP.getResetReason().c_str());
  INFO("Reset info: %s", ESP.getResetInfo().c_str());
  INFO("Free heap: %d B", ESP.getFreeHeap());
  INFO("Chip name: %s", espName);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    logger.info(".");
    delay(500);
  }

  logger.info("Connected to %s! My IP is address: %s",
    STASSID, WiFi.localIP().toString().c_str());

  Udp.begin(8888);
  delay(0);
  logger.info("UDP server open on port 8888");
  delay(0);
  logger.info("Connect to the UDP logger with the following command:");
  delay(0);
  logger.info("nc -luk 8888");
  delay(0);
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

    INFO("Received packet of size %d from %s:%d\t`%s`",
        packetSize,
        Udp.remoteIP().toString().c_str(), Udp.remotePort(),
        packetBuffer);
  }

  static int nextMs = 0;
  const int periodMs = 1000;
  if (millis() > nextMs) 
  {
    nextMs += periodMs;
    logger.info("Duplicate logger still works at %d", millis());
  }
}
