# starlight
[![Build Status](https://ci.appveyor.com/api/projects/status/h8p07uomvf8bx8pe?svg=true)](https://ci.appveyor.com/project/reworks/starlight)
[![Documentation Status](https://travis-ci.org/DomRe/starlight.svg?branch=master)](https://domre.github.io/starlight/)
[![Apache 2.0 Licensed](https://img.shields.io/badge/license-apache-blue.svg)](./LICENSE.txt)

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

### Documentation:  
See [documentation](https://domre.github.io/starlight/) for more.  

### Credits
* [doxygen-dark-theme](https://github.com/MaJerle/doxygen-dark-theme).