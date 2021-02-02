#ifndef UDPLOGGER_H
#define UDPLOGGER_H

#include <ArduinoLogger.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif
#include <WiFiUdp.h>

class UdpLogger : public Logger
{
public:
  UdpLogger(UDP &udp,  IPAddress address, uint16_t port = 8888, const char *context = "DFT", e_log_level level = LOG_ALL);
  
protected:
  void output(const char *log_buffer, size_t len);
  UDP &_udp;
  const IPAddress _address;
  const uint16_t _port;
}; // class Logger

#endif
