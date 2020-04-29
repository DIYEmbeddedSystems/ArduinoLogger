/* simpleLog.ino
 * 
 * Purpose: demonstrate usage of basic logger macros 
 */
#include <Arduino.h>

#include <ArduinoLogger.h>


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  String testString = "This is a test string";

  INFO("Application " __FILE__ " compiled " __DATE__ " at " __TIME__ " started.");

  // Use the DEBUG, INFO, WARNING, ERROR and CRITICAL macros to output printf-formatted messages
  INFO("This is a simple log message");
  INFO("ArduinoLogger supports printf-like formatted messages, eg. %%d: %d", (int)42);
  INFO("And multiple parameter: %d, `%s`, %c", 42, testString.c_str(), '!');

  // Demonstration of macros for various logging levels
  INFO("ArduinoLogger supports multiple logging levels.");
  INFO("This is an information message");
  DEBUG("This is a debug message");
  WARN("This is a warning");
  ERROR("This is an error message");
  CRITICAL("This is a critical error message");

  // Depending on current logging level, messages are printed or not
  SET_LOG_LEVEL(LOG_ERROR); 
  INFO("Messages below the LOG_ERROR level are silenced now");
  WARN("Messages below the LOG_ERROR level are silenced now");
  ERROR("Messages with level >= ERROR are still visible");
  CRITICAL("Messages with level >= ERROR are still visible");
  
  SET_LOG_LEVEL(LOG_ALL);
  INFO("All messages are visible again");

  // Logger discards lengthy messages. Message length can be set by #defining 
  // the LOG_BUFFER_SIZE macro before including <ArduinoLogger.h>
  INFO("Messages of excessive length get silently stripped. 0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........");

  // Tracing utility
  TRACE();
  f();

  // Additional utility: measure the execution time of a given statement or block
  INFO("ArduinoLogger offers time measurement utilies.");
  INFO("How much time does it take to print 10 times a 100-character string?");
  TIME_MS(
    for (int i = 0; i < 10; i++) {
      Serial.println("0.........1.........2.........3.........4.........5.........6.........7.........8.........9.........");
    }
  );

  INFO("How much time it takes to print a complex INFO() statement? Measure in milliseconds");
  TIME_MS({
    INFO("A log message with multiple parameters: int `%d`, string `%s`, charcter `%c`", 42, "this is a string", '!');
  });

  INFO("How much time it takes to print a complex INFO() statement? Measure in microseconds");
  TIME_US({
    INFO("A log message with multiple parameters: int `%d`, string `%s`, charcter `%c`", 42, "this is a string", '!');
  });

  // End of the Logger demonstration
  INFO("That's it folks!");
}

void loop() {
  delay(1000);
  INFO("Still alive...");
}

void f(void) {
  TRACE();
  g();
}

void g(void) {
  TRACE();
  h();
}

void h(void) {
  TRACE();
}

