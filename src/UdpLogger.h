#ifndef UDPLOGGER_H
#define UDPLOGGER_H

#include <ArduinoLogger.h>
#include <ESP8266WiFi.h>
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
