#include <Arduino.h>
#include <ArduinoLogger.h>
#include <DupLogger.h>

DupLogger::DupLogger(Logger &logger1, Logger &logger2)
    : _logger1(logger1), _logger2(logger2) 
{
  info("DupLogger constructed");
}


void DupLogger::output(const char *log_buffer, size_t len)
{
  _logger1.output(log_buffer, len);
  _logger2.output(log_buffer, len);
}
