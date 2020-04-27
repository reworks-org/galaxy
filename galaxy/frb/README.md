# frb

Modern C++ OpenAL audio library.

## Basic Usage
```cpp
frb::Context context;
context.initialize();

frb::Audible music;
music.loadFromFile("bin/test.ogg");
music.play();
	
std::cin.get();
music.stop();
```