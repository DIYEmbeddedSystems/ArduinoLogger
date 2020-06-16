#ifndef TELNETLOGGER_H
#define TELNETLOGGER_H

#include <ArduinoLogger.h>
#include <ESP8266WiFi.h>


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
