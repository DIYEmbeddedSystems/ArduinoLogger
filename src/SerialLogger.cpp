
#include <Arduino.h>

#include <SerialLogger.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// constructor
SerialLogger::SerialLogger(Print &printer, const char *context, e_log_level level)
  : Logger(context, level)
  , _printer(printer)
{
  debug("SerialLogger constructed");
}

// singleton accessor for default logger
SerialLogger &SerialLogger::getDefault() 
{ 
  static SerialLogger defaultLogger(Serial, (const char*)"DFT", LOG_ALL);
  if (!Serial) {
    Serial.begin(115200);
    while (!Serial);
  }
  return defaultLogger;
}



// the actual output function
void SerialLogger::output(const char *log_buffer, size_t len)
{
    _printer.println(log_buffer);
}