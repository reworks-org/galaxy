///
/// testbed.cpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

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