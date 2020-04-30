#ifndef DUPLOGGER_H
#define DUPLOGGER_H

#include <ArduinoLogger.h>

class DupLogger : public Logger {
public:
    DupLogger(Logger &logger1, Logger &logger2);
    void enable(uint8_t which, bool enable);

protected:
  void output(const char *log_buffer, size_t len);
  Logger &_logger1;
  Logger &_logger2;
};

#endif
