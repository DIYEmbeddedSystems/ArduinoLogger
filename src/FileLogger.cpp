#include <FileLogger.h>
#include <Arduino.h>


FileLogger::FileLogger(File &file, const char *context, e_log_level level) :
    Logger(context, level), _file(file)
{
  debug("FileLogger constructed");
}

// the actual output function
void FileLogger::output(const char *log_buffer, size_t len)
{
  _file.write(log_buffer, len);
  _file.write('\r');
  _file.write('\n');
}
