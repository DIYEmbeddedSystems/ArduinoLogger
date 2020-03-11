#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <Arduino.h>

#include <cstdarg>
#include <stdio.h>
#include <string.h>

enum e_log_level{
  LOG_OFF = 0,
  LOG_CRITICAL,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG,
  LOG_ALL = LOG_DEBUG,
  LOG_LEVEL_MAX = LOG_ALL
};


#define BUF_SIZE 1024


class Logger 
{
public:
  Logger(const char *context, Print &printer = Serial, e_log_level level = LOG_ALL);
  
  static Logger &getDefault();
  
  void log(e_log_level level, const char* fmt, ...);
  void critical(const char* fmt, ...);
  void error(const char* fmt, ...);
  void warn(const char* fmt, ...);
  void info(const char* fmt, ...);
  void debug(const char* fmt, ...);
  void trace(const char *file, int line);

protected:
  void vlog(int level, const char *fmt, va_list ap);

private:
  e_log_level _logLevel;
  Print &_printer;
  const char *_context;
}; // class Logger



#ifndef NDEBUG

#define __FILENAME__        ((strrchr(__FILE__, '\\' )) ? \
                            (1+strrchr(__FILE__, '\\' )) : \
                                ((strrchr(__FILE__, '/')) ? \
                                (1+strrchr(__FILE__, '/')) : \
                                (__FILE__)) )

#define TRACE()           Logger::getDefault().trace(__FILENAME__, __LINE__)
#define LOG(msg...)       Logger::getDefault().log(LOG_DEBUG, msg)
#define DEBUG(msg...)     Logger::getDefault().log(LOG_DEBUG, msg)
#define INFO(msg...)      Logger::getDefault().log(LOG_INFO, msg)
#define WARN(msg...)      Logger::getDefault().log(LOG_WARNING, msg)
#define ERROR(msg...)     Logger::getDefault().log(LOG_ERROR, msg)
#define CRITICAL(msg...)  Logger::getDefault().log(LOG_CRITICAL, msg)

#define TIME_US(block)    do { \
                            unsigned startUs = micros(); \
                            do { block ;} while (0); \
                            unsigned endUs = micros(); \
                            DEBUG("Time : %s (%d) took %d us", __FILENAME__, __LINE__,  endUs - startUs); \
                          } while (0)

#else

#define TRACE()
#define LOG(msg...)
#define INFO(msg...)
#define WARN(msg...)
#define ERROR(msg...)
#define CRITICAL(msg...)

#define TIME_US(block)   do { block; } while(0)

#endif


#endif