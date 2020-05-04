/**
 * This code shows how you can create and use multiple logger contexts.  
 *
 */
#include <Arduino.h>
#include <FileLogger.h>
#include <FS.h>

// default logger (not instantiated here) uses Serial for output, and logs all messages
File logFile;
FileLogger *logger;

void setup() {
  Serial.begin(115200);
  delay(1000);
 
  INFO("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " Starting...");
  
  SPIFFS.begin();
  logFile = SPIFFS.open("log.txt", "a+"); // append to file, and read from file

  logger = new FileLogger(logFile, "FILE", LOG_ALL);

  // Trace execution with default logger, or with specific loggers.
  TRACE();
  LOGGER_TRACE(*logger);
 
  // DEBUG-level messages
  DEBUG("This is a debug message");
  logger->debug("This is a debug message to log onto file");
  
}

void loop() {
  heartbeat();
}


void heartbeat() {
  static uint32_t nextMs = 0;
  const uint32_t periodMs = 5000;

  if ((int32_t)(millis() - nextMs) >= 0) {
    nextMs += periodMs;
    logger->info("Still alive at %d", millis());
    
    if (logFile) {
      INFO("----Reading log file----");
      logFile.seek(0, SeekSet); // set cursor to beginning of file
      while (logFile.available()) {
        String line = logFile.readStringUntil('\r');
        INFO(">%s", line.c_str());
      }
      INFO("----");
    }
    logFile.seek(0, SeekEnd); // move cursor at end of file, for logger
  }
}

