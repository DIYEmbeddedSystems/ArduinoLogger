
#include <Arduino.h>

#include <ArduinoLogger.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// short identifier for logging level
static const char log_level_prefix[LOG_LEVEL_MAX + 1] = {
  '_', '?', '!', '#', 'i', '+'
};

// full name of logging level
static const char *log_level_name[LOG_LEVEL_MAX + 1] = {
  "_", "CRITICAL", "ERROR", "WARNING", "info", "debug"
};

// constructor
Logger::Logger(const char *context, Print &printer, e_log_level level)
  : _context(context) 
  , _printer(printer)
  , _logLevel(level)
  , _counter(0)
{
  debug("Logger constructed");
}

// singleton accessor for default logger
Logger &Logger::getDefault() 
{ 
  static Logger defaultLogger((const char*)"DFT", Serial, LOG_ALL);
  if (!Serial) {
    Serial.begin(115200);
    while (!Serial);
  }
  return defaultLogger;
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


// the actual output function
void Logger::vlog(int level, const char *fmt, va_list ap)
{
  int offset = 0;
  char buffer[LOG_BUFFER_SIZE];

  // process message only if current logging level is below message level
  if (level <= _logLevel)
  {
/* Here, we hope there is no rollover between call to millis() and subsequent 
call to micros(). However log timestamping coherency is not critical enough to
justify the burden of a coherent clock sampling procedure. */
    long unsigned int ms = millis(); 
    long unsigned int us = micros();
    unsigned int s = ms / 1000;
    unsigned int min = s / 60;

    // print log prefix to buffer
    offset += snprintf(buffer, sizeof(buffer), 
        "[%02u:%02u.%03u%03u\t", 
        (unsigned int)min, (unsigned int)s % 60, 
        (unsigned int)ms % 1000, (unsigned int)us % 1000);
    
    offset += snprintf(buffer + offset, sizeof(buffer) - offset,
        "<%s> %c %02u]\t", (const char*)_context, 
        (char)log_level_prefix[level], (unsigned int)_counter);
        
    // print user message to buffer
    offset += vsnprintf(buffer + offset, sizeof(buffer) - offset, 
        fmt, ap);

    // output message buffer
    _printer.println(buffer);
  }

  // message counter is incremented even when message is not actually printed
  ++_counter;
}


