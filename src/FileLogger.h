#include <ArduinoLogger.h>
#include <Arduino.h>
#include <FS.h>

class FileLogger : public Logger
{
public:
  FileLogger(File &file, const char *context = "DFT", e_log_level level = LOG_ALL);
  
protected:
  void output(const char *log_buffer, size_t len);
  File &_file;
}; // class Logger
