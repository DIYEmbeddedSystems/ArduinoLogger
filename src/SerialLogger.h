#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include <ArduinoLogger.h>

class SerialLogger : public Logger
{
public:
  SerialLogger(Print &printer = Serial, const char *context = "DFT", e_log_level level = LOG_ALL);
  static SerialLogger &getDefault();
  
protected:
  void output(const char *log_buffer, size_t len);
  Print &_printer;
 }; // class SerialLogger


#endif