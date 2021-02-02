#ifndef TELNETLOGGER_H
#define TELNETLOGGER_H

#include <ArduinoLogger.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif


// how many bytes of log messages to retain while no client is connected
#define MAX_BUFFER_LENGTH 1024

class TelnetLogger : public Logger
{
public:
  TelnetLogger(uint16_t port = 23, const char *context = "Telnet", e_log_level level = LOG_ALL);

  boolean isClientConnected();

//protected:
  void output(const char *log_buffer, size_t len);

  WiFiServer _server;
  WiFiClient _client;
  String _buffered;
}; // class Logger

#endif
