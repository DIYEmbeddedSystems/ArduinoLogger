/**
 * This code shows how you can create and use multiple logger contexts.  
 *
 */
#include <Arduino.h>

#include "Logger.hpp"


// default logger (not instantiated here) uses Serial for output, and logs all messages
// logger1 context uses Serial for output, logs only messages above the WARNING level
// logger2 context uses Serial for output, logs only messages above the INFO level
Logger logger1("CTX1", Serial, LOG_WARNING);
Logger logger2("CTX2", Serial, LOG_INFO);


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  LOG("Starting...");
  
  // Trace execution with default logger, or with specific loggers.
  TRACE();
  logger1.trace(__FILENAME__, __LINE__);
  logger2.trace(__FILENAME__, __LINE__);
  
  // DEBUG-level messages
  LOG("This is a log message");
  DEBUG("This is a debug message");
  logger1.debug("This is a debug message");
  logger2.debug("This is a debug message");
  
  // INFO-level messages
  INFO("This is an information message");
  logger1.info("This is an information message");
  logger2.info("This is an information message");
  
  // WARNING-level messages
  WARN("This is a warning");
  logger1.warn("This is a warning");
  logger2.warn("This is a warning");
  
  // ERROR-level messages
  ERROR("This is an error message!");
  logger1.error("This is an error message!");
  logger2.error("This is an error message!");
  
  // CRITICAL-level messages
  CRITICAL("This is a critical error !!");
  logger1.critical("This is a critical error !!");
  logger2.critical("This is a critical error !!");
}

void loop() {  
  TRACE();
  delay(1000);
}


