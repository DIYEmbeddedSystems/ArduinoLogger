
#include <Arduino.h>

#include "Logger.hpp"


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  LOG("Starting...");
  
  TRACE();

  LOG("This is a log message");
  LOG("This is a log message with integer parameter: %d", (int)42);
  LOG("This is a log message with string parameter: `%s`", F("this string is stored in program memory"));
  LOG("This is a log message with multiple parameter: %d, `%s`, %c", 42, F("progmem string"), '!');
  DEBUG("This is a debug message");
  INFO("This is an information message");
  WARN("This is a warning");
  ERROR("This is an error message!");
  CRITICAL("This is a critical error !!");
}

void loop() {
  TRACE();  
  INFO("How much time does function f take?");
  TIME_US(f(4));
  delay(1000);
}

void f(int n) {
  LOG("message with parameter: n = %d", n);
  if (n > 0) {
    f(n-1);
  }
}

