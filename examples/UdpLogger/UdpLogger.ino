/**
 * This code shows how you can create and use multiple logger contexts.  
 *
 */


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#include <ArduinoLogger.h>
#include <UdpLogger.h>


#define STASSID "your-AP"
#define STAPSK  "your-password"

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "ack\r\n";       // a string to send back

WiFiUDP Udp;

UdpLogger logger(Udp, "UDP", LOG_ALL);


void setup() {
  Serial.begin(115200);
  
  Serial.println("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " started.");
  INFO("Default logger OK");

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    INFO(".");
    delay(500);
  }

  INFO("Connected to %s! My IP is address: %s",
    STASSID, WiFi.localIP().toString().c_str());

  INFO("UDP server open on port %d", localPort);
  Udp.begin(localPort);
  logger.info("Start!!");
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;

    INFO("Received packet of size %d from %s:%d\t `%s`",
                  packetSize,
                  Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                  packetBuffer);
  }

  static int nextMs = 0;
  const int periodMs = 1000;
  if (millis() > nextMs) {
    nextMs += periodMs;
    logger.info("Hey simple packet works at %d", millis());
    INFO("Sent timple packet");

  }
}

/*
  test (shell/netcat):
  --------------------
	  nc -u 192.168.esp.address 8888
*/
