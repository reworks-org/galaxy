# pulsar

Minimal multi-threaded C++17 logging library.

# Tutorial
### Setup
Logging class is accessed through a singleton instance:
```cpp
pl::Log::i()
``` 
To set up library:
```cpp
pl::Log::i().init("logs/logs.txt");
```
The path is where the log file will be stored and all output written to.

There are 5 different logged message levels:
INFO, DEBUG, WARNING, ERROR & FATAL.

The logging class provides a method to filter out these messages. Useful for debug/release builds. This includes console and text file output.
```cpp
pl::Log::i().setMinimumLevel(pl::Log::Level::INFO)
```
will mean that all messages from INFO and up will be logged.
```cpp
pl::Log::i().setMinimumLevel(pl::Log::Level::WARNING)
```
will only log messages from WARNING and up - INFO and DEBUG messages will not be logged.
Minimum level can be retrieved with 
```cpp
pl::Log::Level level = pl::Log::i().getMinimumLevel();
```

### Logging
Logging is done simply through an extensible macro.
```cpp
LOG_S(x)
```
_S refers to the log utilizing a stream and the 'x' argument is the log level. Does not automatically newline so make sure to include '\n' to newline.
```cpp
LOG_S(pl::Log::Level::WARNING) << "Failed to load textures/background.png\n";
```


The log class also provides a convenient way to get the current date and time in a string format.
```cpp
std::string dateTime = pl::Log::i().getDateTime();
```

### Complete example
```cpp
#include <pl/Log.hpp>

int main(int argc, char** argv)
{
	pl::Log::i().init("logs/logfile.txt");
	pl::Log::i().setMinimumLevel(pl::Log::Level::WARNING);

	LOG_S(pl::Log::Level::INFO) << "Should not log.\n";
	LOG_S(pl::Log::Level::WARNING) << "Should Log.";

	std::cin.get();
	
	return 0;
}
```