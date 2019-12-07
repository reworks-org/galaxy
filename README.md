# starlight
[![Build Status](https://ci.appveyor.com/api/projects/status/f9ym5sddth68wp78?svg=true)](https://ci.appveyor.com/project/reworks/starlight)

Simple C++ Event register and dispatcher.

## Usage:
```cpp
struct Test
{
	int a = 10;
};

int main()
{
	Test test;
	starlight::Dispatcher dispatcher;

	dispatcher.add<Test>([](const Test& test)
	{
		std::cout << test.a << std::endl;
	});

	dispatcher.trigger<Test>(test);

	std::cin.get();
}
```