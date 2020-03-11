
#include <Arduino.h>

#include "Logger.hpp"

#include <cstdarg>
#include <stdio.h>
#include <string.h>

static const char *log_level_prefix[LOG_LEVEL_MAX + 1] = {
  ".", "?", "!", "W", "i", "d"
};

static const char *log_level_name[LOG_LEVEL_MAX + 1] = {
  "off", "CRITICAL", "ERROR", "WARNING", "info", "debug"
};


Logger::Logger(const char *context, Print &printer, e_log_level level)
  : _context(context)
  , _printer(printer)
  , _logLevel(level)
{
  _printer.print("Logger created: "); _printer.println(context);
}

Logger &Logger::getDefault() 
{ 
  static Logger logger("DFT", Serial, LOG_ALL);
  if (!Serial) {
    Serial.begin(115200);
    while (!Serial);
  }
  return logger;
}
  
void Logger::log(e_log_level level, const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(level, fmt, ap);
  va_end(ap);
}

void Logger::critical(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_CRITICAL, fmt, ap);
  va_end(ap);
}

void Logger::error(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_ERROR, fmt, ap);
  va_end(ap);
}

void Logger::warn(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_WARNING, fmt, ap);
  va_end(ap);
}

void Logger::info(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_INFO, fmt, ap);
  va_end(ap);
}

void Logger::debug(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_DEBUG, fmt, ap);
  va_end(ap);
}

void Logger::trace(const char *file, int line)
{
  log(LOG_DEBUG, "%s:%d", file, line);
}


void Logger::vlog(int level, const char *fmt, va_list ap)
{
  int offset = 0;
  char buffer[BUF_SIZE];
  const char *file = "NOFILE";
  const int line = 9999;

  if (level <= _logLevel)
  {
    long int ms = millis(); /* just hope there is no rollover between call to millis() and subsequent call to micros() */
    long int us = micros() % 1000000;
    int s = ms / 1000;
    ms %= 1000;
    int min = s / 60;
    s %= 60;

    offset += snprintf(buffer, sizeof(buffer), 
      "[%02d:%02d.%06d\t <%s>\t %s]\t ", min, s, us, log_level_prefix[level], _context);

    offset += vsnprintf(buffer + offset, sizeof(buffer) - offset,
      fmt, ap);

    _printer.println(buffer);
  }
}

