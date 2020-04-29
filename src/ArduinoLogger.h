/* Logger library for Arduino

This library generates a stream of timestamped, contextualized message logs
logging format: 
[mm:ss.uuuuuu    <DFT>   !    12] User-specific message format
 |                |      |     |    +- user-specified message
 |                |      |     +- message counter; for detection of lost messages
 |                |      +- logging context mnemonic
 |                +- mnemonic of message logging level
 +- timestamp, in the form minutes:seconds.microseconds 

Copyright DIY Embedded Systems 2020
Code under MIT License
*/

#ifndef ARDUINOLOGGER_HPP
#define ARDUINOLOGGER_HPP

#include <Arduino.h>  // Print and Serial

#include <stdarg.h>   // variadic macros
//#include <stdio.h>    
#include <string.h>   // strrchr

// You can override the default buffer size by #defining this macro in your application code, 
// before including <ArduinoLogger.h>
#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 128
#endif


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
  void trace(const char *file, const char *function, int line);
  enum e_log_level getLevel();
  void setLevel(enum e_log_level level);

protected:
  void vlog(int level, const char *fmt, va_list ap);
  e_log_level _logLevel;
  Print &_printer;
  const char *_context;
  unsigned int _counter;
}; // class Logger



#ifndef NDEBUG

// Quick hack to strip the full path from the __FILE__ macro. 
// TODO: replace with compile-time evaluation mechanism to 
// save runtime performance overhead.
#define FILENAME        ((strrchr(__FILE__, '\\' )) ? \
                            (1+strrchr(__FILE__, '\\' )) : \
                                ((strrchr(__FILE__, '/')) ? \
                                (1+strrchr(__FILE__, '/')) : \
                                (__FILE__)) )


// macro shorthand to use default logger
#define TRACE()           LOGGER_TRACE(Logger::getDefault())
#define LOG(msg...)       Logger::getDefault().log(LOG_DEBUG, msg)
#define DEBUG(msg...)     Logger::getDefault().log(LOG_DEBUG, msg)
#define INFO(msg...)      Logger::getDefault().log(LOG_INFO, msg)
#define WARN(msg...)      Logger::getDefault().log(LOG_WARNING, msg)
#define ERROR(msg...)     Logger::getDefault().log(LOG_ERROR, msg)
#define CRITICAL(msg...)  Logger::getDefault().log(LOG_CRITICAL, msg)

#define SET_LOG_LEVEL(x) Logger::getDefault().setLevel(x)

// use this macro to trace with a specific logger
#define LOGGER_TRACE(logger) (logger).trace(FILENAME, __PRETTY_FUNCTION__, __LINE__)

// macros to measure execution time in microseconds, and milliseconds.
#define TIME_US(block)    do { \
                            unsigned int startUs = micros(); \
                            do { block ;} while (0); \
                            unsigned int endUs = micros(); \
                            DEBUG("TIME: block at %s:%d took %u us", FILENAME, __LINE__,  endUs - startUs); \
                          } while (0)

#define TIME_MS(block)    do { \
                            unsigned int startMs = millis(); \
                            do { block ;} while (0); \
                            unsigned int endMs = millis(); \
                            DEBUG("TIME: block at %s:%d took %u ms", FILENAME, __LINE__,  endMs - startMs); \
                          } while (0)


#else

#define TRACE()
#define LOG(msg...)
#define INFO(msg...)
#define WARN(msg...)
#define ERROR(msg...)
#define CRITICAL(msg...)

#define TIME_US(block)   do { block; } while(0)
#define TIME_MS(block)   do { block; } while(0)

#endif


#endif