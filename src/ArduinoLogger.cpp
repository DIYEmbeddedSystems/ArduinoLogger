
#include <Arduino.h>

#include <ArduinoLogger.h>
#include <SerialLogger.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


// constructor
Logger::Logger(const char *context, e_log_level level)
    : _logLevel(level), _context(context), _counter(0)
{
}

Logger &Logger::getDefault()
{
  return SerialLogger::getDefault();
}


// generic logging function
void Logger::log(e_log_level level, const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(level, fmt, ap);
  va_end(ap);
}

// critical error-level logging
void Logger::critical(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_CRITICAL, fmt, ap);
  va_end(ap);
}

// error-level logging
void Logger::error(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_ERROR, fmt, ap);
  va_end(ap);
}

// warning-level logging
void Logger::warn(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_WARNING, fmt, ap);
  va_end(ap);
}

// info-level logging
void Logger::info(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_INFO, fmt, ap);
  va_end(ap);
}

// debug-level logging
void Logger::debug(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_DEBUG, fmt, ap);
  va_end(ap);
}

// hexdump
void Logger::hexdump(const void *data, size_t len)
{
  const uint8_t *buff = static_cast<const uint8_t *>(data);
  const int HEXDUMP_LINE_SIZE = 16;

  char line[200];
  uint32_t idx;

  idx = 0;
  for (uint32_t offset = 0; offset < len; offset += HEXDUMP_LINE_SIZE)
  {
    idx = 0;
    for (uint32 i = 0; i < HEXDUMP_LINE_SIZE; i++)
    {
      if (offset + i < len) 
      {
        idx += snprintf(line + idx, sizeof(line) - idx, 
            "%02x ", buff[offset + i]);
      }
      else
      {
        idx += snprintf(line + idx, sizeof(line) - idx, "   ");
      }
    }

    idx += snprintf(line + idx, sizeof(line) - idx, " | ");

    for (uint32 i = 0; i < HEXDUMP_LINE_SIZE; i++)
    {
      char c = (offset + i < len) ? buff[offset + i] :' ';
      idx += snprintf(line + idx, sizeof(line) - idx, 
          "%c", isprint(c) ? c : '.');
    }

    debug("HEXDUMP %s", line);
  }
}

// tracing (at debug level)
void Logger::trace(const char *file, const char *function, int line)
{
  log(LOG_DEBUG, "TRACE: %s:%u (%s)", file, line, function);
}

enum e_log_level Logger::getLevel()
{
  return _logLevel;
}

void Logger::setLevel(enum e_log_level level)
{
  _logLevel = level;
}

void Logger::setContext(const char *context)
{
  _context = context;
}

void Logger::vlog(int level, const char *fmt, va_list ap)
{
  char buffer[LOG_BUFFER_SIZE];
  uint32_t len = 0;

  // process message only if current logging level is below message level
  if (level <= _logLevel)
  {
/* Here, we hope there is no rollover between call to millis() and subsequent
call to micros(). However log timestamping coherency is not critical enough to
justify the burden of a coherent clock sampling procedure. */
    long unsigned int ms = millis();
#ifdef LOGGER_TSTAMP_MICROSECOND
    long unsigned int us = micros();
#endif
    unsigned int s = ms / 1000;
    unsigned int min = s / 60;
    unsigned int h = min / 60;
    ms %= 1000;
    s %= 60;
    min %= 60;

    // print log prefix to buffer
    len += snprintf(buffer, sizeof(buffer),
        "[%02u:%02u:%02u.%03u",
        (unsigned int)h, (unsigned int)min, (unsigned int)s,
        (unsigned int)ms);

#ifdef LOGGER_TSTAMP_MICROSECOND
    us %= 1000;
    len += snprintf(buffer, sizeof(buffer) - len,
        "%03u", (unsigned int)us);
#endif

    len += snprintf(buffer + len, sizeof(buffer) - len,
        "\t<%s> %c\t%03u]\t", (const char*)_context,
        (char)log_level_prefix[level], (unsigned int)_counter);

    // print user message to buffer
    len += vsnprintf(buffer + len, sizeof(buffer) - len,
        fmt, ap);

    // if buffer overflowed: mark truncation
    if (len >= sizeof(buffer))
    {
      buffer[sizeof(buffer)-4] = buffer[sizeof(buffer)-3] = buffer[sizeof(buffer)-2] = '~';
    }

    // output message buffer
    output(buffer, len);
  }

  // message counter is incremented even when message is not actually printed
  ++_counter;
}


