

# ArduinoLogger
A library with logging and tracing utilities for the Arduino environment, with a uniform log format featuring a timestamp, a logging context, a message counter, and printf-formatted user messages.

Several logging backends are provided:
* SerialLogger sends log messages to any serial line (defaults to the `Serial`)
* UdpLogger sends log messages to a server listening on a given UDP port

# Support
This code was (manually) tested on Arduino Mega and ESP8266, it is expected to work within most other Arduino platforms.

# Features
## Formatted log stream 
The log format is shown below.
```log
[00:01.041072	<DFT> i 07]	This is an information message
[00:01.046004	<DFT> + 08]	This is a debug message
[00:01.050804	<DFT> # 09]	This is a warning
[00:01.054628	<DFT> ! 10]	This is an error message
[00:01.058048	<DFT> ? 11]	This is a critical error message
[00:01.098232	<DFT> + 19]	TRACE: simpleLog.ino:80 (void f())
[00:01.215120	<DFT> + 24]	TIME: block at simpleLog.ino:58 took 87 ms
```

Detail of the log format:
```log
[mm:ss.uuuuuu    <DFT>   !    12] User-specific message format
 |                |      |     |    +- user-specified message, with printf semantics
 |                |      |     +- message counter
 |                |      +- logging context mnemonic
 |                +- mnemonic of message logging level
 +- timestamp, in the form minutes:seconds.microseconds 
```

## Tracing
A simple easy to use macro for tracing program execution. When your program encounters the `TRACE()` macro somewhere in some function, the log stream will contain a log element of the form:   

```log
[00:01.098232	<DFT> + 19]	TRACE: simpleLog.ino:80 (void f())
                            |      |             |   +- function signature
                            |      |             +- line number
                            |      +- file name
                            +- indicates a TRACE statement
```

## Execution time measurements
You can easily measure how much time was spent in some code block. Just type `TIME_MS({your code;})` around some code block, and the execution time of your block will be printed out in the form:
```log
[00:01.215120	<DFT> + 24]	TIME: block at simpleLog.ino:58 took 87 ms
                            |              |             |       +- execution time in milliseconds
                            |              |             +- line number
                            |              +- file name
                            +- indicates a TIME statement
```

Two variants are provided:
* `TIME_MS(...)` measures execution time in milliseconds, 
* macro `TIME_US(...)` measures execution time in microseconds.

# Usage

### Notes & limitations
* The timestamp, in the form `minutes:seconds.microseconds` is generated from the millis() and micros() apis; therefore may be subject to integer rollover after at least 71 minutes of run time. 
* Moreover, the first 3 digits of the `uuuuuu` part come from millis(), while the last 3 digits come from micros(). You may therefore experience (very rarely) incoherencies in the following situation:
```
at time 12.345999 : millis() = 12345
...
at time 12.346001 : micros() = 001
```
In this situation, any value between 12.34599 and 12.346001 would be OK, however the generated timestamp will be `12.345001`, which is incorrect, e.g. it could be generated *after* the timestamp 12.345678. This phenomenon should however happen very infrequently, therefore not justify the overhead of a coherent clock reading mechanism.
* the `TIME_US(block)` macro measurement is not cycle-accurate, i.e. the macro may add a couple microseconds to the block. For better accuracy if needed, you can compensate by substracting the execution time of an empty block given by `TIME_US({})`.


