#include <UdpLogger.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

UdpLogger::UdpLogger(UDP &udp, const char *context, e_log_level level) :
    LoggerBase(context, level), _udp(udp)
{
    Serial.println("UdpLogger created");
}

// the actual output function
void UdpLogger::vlog(int level, const char *fmt, va_list ap)
{
  int len = 0;
  char buffer[LOG_BUFFER_SIZE];

  // process message only if current logging level is below message level
  if (level <= _logLevel 
      && _udp.beginPacket(_udp.remoteIP(), _udp.remotePort()))
  {
/* Here, we hope there is no rollover between call to millis() and subsequent 
call to micros(). However log timestamping coherency is not critical enough to
justify the burden of a coherent clock sampling procedure. */
    long unsigned int ms = millis(); 
    long unsigned int us = micros();
    unsigned int s = ms / 1000;
    unsigned int min = s / 60;

    // print log prefix to buffer
    len += snprintf(buffer, sizeof(buffer), 
        "[%02u:%02u.%03u%03u\t", 
        (unsigned int)min, (unsigned int)s % 60, 
        (unsigned int)ms % 1000, (unsigned int)us % 1000);
    
    len += snprintf(buffer + len, sizeof(buffer) - len,
        "<%s> %c %02u]\t", (const char*)_context, 
        (char)log_level_prefix[level], (unsigned int)_counter);
        
    // print user message to buffer
    len += vsnprintf(buffer + len, sizeof(buffer) - len, 
        fmt, ap);

    len += snprintf(buffer + len, sizeof(buffer) - len,
        "\r\n");

    // output message buffer
    _udp.write(reinterpret_cast<const uint8_t *>(buffer), len);
    _udp.endPacket();
  }

  // message counter is incremented even when message is not actually printed
  ++_counter;
}
