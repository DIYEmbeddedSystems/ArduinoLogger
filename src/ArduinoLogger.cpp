
#include <Arduino.h>

#include <ArduinoLogger.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>



/* LoggerBase */
// constructor
LoggerBase::LoggerBase(const char *context, e_log_level level)
    : _context(context), _logLevel(level), _counter(0)
{
}

LoggerBase &LoggerBase::getDefault() 
{
  return SerialLogger::getDefault();
}


// generic logging function  
void LoggerBase::log(e_log_level level, const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(level, fmt, ap);
  va_end(ap);
}

// critical error-level logging
void LoggerBase::critical(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_CRITICAL, fmt, ap);
  va_end(ap);
}

// error-level logging
void LoggerBase::error(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_ERROR, fmt, ap);
  va_end(ap);
}

// warning-level logging
void LoggerBase::warn(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_WARNING, fmt, ap);
  va_end(ap);
}

// info-level logging
void LoggerBase::info(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_INFO, fmt, ap);
  va_end(ap);
}

// debug-level logging
void LoggerBase::debug(const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  vlog(LOG_DEBUG, fmt, ap);
  va_end(ap);
}

// tracing (at debug level)
void LoggerBase::trace(const char *file, const char *function, int line)
{
  log(LOG_DEBUG, "TRACE: %s:%u (%s)", file, line, function);
}

enum e_log_level LoggerBase::getLevel() 
{
  return _logLevel;
}

void LoggerBase::setLevel(enum e_log_level level) 
{
  _logLevel = level;
}



// constructor
SerialLogger::SerialLogger(const char *context, e_log_level level, Print &printer)
  : LoggerBase(context, level)
  , _printer(printer)
{
  debug("Logger constructed");
}

// singleton accessor for default logger
SerialLogger &SerialLogger::getDefault() 
{ 
  static SerialLogger defaultLogger((const char*)"DFT", LOG_ALL, Serial);
  if (!Serial) {
    Serial.begin(115200);
    while (!Serial);
  }
  return defaultLogger;
}



// the actual output function
void SerialLogger::vlog(int level, const char *fmt, va_list ap)
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


