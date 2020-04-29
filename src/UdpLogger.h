#include <ArduinoLogger.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class UdpLogger : public LoggerBase
{
public:
  UdpLogger(UDP &udp, const char *context = "DFT", e_log_level level = LOG_ALL);
  
protected:
  void vlog(int level, const char *fmt, va_list ap);
  UDP &_udp;
}; // class Logger