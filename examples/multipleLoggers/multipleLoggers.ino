/**
 * This code shows how you can create and use multiple logger contexts.  
 *
 */
#include <Arduino.h>
#include <ArduinoLogger.h>


// default logger (not instantiated here) uses Serial for output, and logs all messages
// logger1 context, for main program, uses Serial for output, logs only messages above the WARNING level
// logger2 context, for a specific function, uses Serial for output, logs only error and critical messages

Logger loggerProg("PROG", Serial, LOG_WARNING);
Logger loggerFunc("FUNC", Serial, LOG_ERROR);


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  LOG("Starting...");
  
  // Trace execution with default logger, or with specific loggers.
  TRACE();
  LOGGER_TRACE(loggerProg);
  LOGGER_TRACE(loggerFunc);
  
  // DEBUG-level messages
  DEBUG("This is a debug message");
  loggerProg.debug("This is a debug message");
  loggerFunc.debug("This is a debug message");
  
  // INFO-level messages
  INFO("This is an information message");
  loggerProg.info("This is an information message");
  loggerFunc.info("This is an information message");
  
  // WARNING-level messages
  WARN("This is a warning");
  loggerProg.warn("This is a warning");
  loggerFunc.warn("This is a warning");
  
  // ERROR-level messages
  ERROR("This is an error message!");
  loggerProg.error("This is an error message!");
  loggerFunc.error("This is an error message!");
  
  // CRITICAL-level messages
  CRITICAL("This is a critical error !!");
  loggerProg.critical("This is a critical error !!");
  loggerFunc.critical("This is a critical error !!");
}

void loop() {
  loggerFunc.setLevel((enum e_log_level)(((int)loggerFunc.getLevel() + 1) % LOG_ALL));
  INFO("loggerFunc logging level is now set to %d", loggerFunc.getLevel());
  loggerProg.debug("This is a debug message");
  loggerFunc.debug("This is a debug message");
  loggerProg.info("This is an information message");
  loggerFunc.info("This is an information message");
  loggerProg.warn("This is a warning");
  loggerFunc.warn("This is a warning");
  loggerProg.error("This is an error message!");
  loggerFunc.error("This is an error message!");
  loggerProg.critical("This is a critical error !!");
  loggerFunc.critical("This is a critical error !!");
  INFO("\n\n\n");
  delay(1000);
}


