///
/// testbed.cpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include "nova/event/Base.hpp"
#include "nova/event/Wrapper.hpp"
#include "nova/Dispatcher.hpp"

struct Test
{
	int a = 10;
};

int main()
{
	Test test;
	nova::Dispatcher dispatcher;

	dispatcher.add<Test>([](const Test& test)
	{
		std::cout << test.a << std::endl;
	});

	dispatcher.trigger<Test>(test);

	std::cin.get();
}