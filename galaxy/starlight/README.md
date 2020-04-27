# starlight

Simple multithreaded C++17 Event register and dispatcher.

## Usage:
```cpp
#include <iostream>

#include "starlight/Dispatcher.hpp"

struct Test
{
	int a = 10;
};

int main()
{
	Test test;
	starlight::Dispatcher dispatcher;

	dispatcher.add<Test>([](const Test& test_int, std::mutex& mutex)
	{
		mutex.lock();

		std::cout << test_int.a << std::endl;

		mutex.unlock();
	});

	dispatcher.trigger<Test>(test);

	std::cin.get();
}
```