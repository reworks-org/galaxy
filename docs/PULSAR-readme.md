# pulsar

Minimal multi-threaded C++20 logging library.

# Tutorial
### Setup
Logging class is accessed through a singleton instance:
```cpp
pl::Log::i()

// Or through macro:
PL_LOG_GET
``` 
But most functions of logging class have a macro. To set up library:
```cpp
PL_LOG_START(log_file); // log_file is file to log to. Can include path.

// Then to close when done (required).
PL_LOG_FINISH;
```
The path is where the log file will be stored and all output written to.

There are 5 different logged message levels:
INFO, DEBUG, WARNING, ERROR & FATAL.

Macros are as follows:
```cpp
PL_INFO
PL_DEBUG
PL_WARNING
PL_ERROR
PL_FATAL
```

### Logging
Logging is done simply through an extensible macro.
```cpp
PL_LOG(...)
```
Logging takes in variadic arguments in the following order: LEVEL, MESSAGE, VARIABLES. With MESSAGE supporting std::format formatting.
```cpp
PL_LOG(PL_INFO, "This is the: {0}.", "readme");
```

The log class also provides a convenient way to toggle console logging when running unit tests:
```cpp
PL_ENABLE_TESTING_MODE;
PL_DISABLE_TESTING_MODE;
```
