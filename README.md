# starmap
[![Build Status](https://ci.appveyor.com/api/projects/status/yigrhkcvgofvhhwt?svg=true)](https://ci.appveyor.com/project/reworks/starmap)
[![Documentation Status](https://travis-ci.org/DomRe/starmap.svg?branch=master)](https://domre.github.io/starmap/)

C++17 JSON Tiled Map Parser.

## Basic Usage

```cpp
#include <starmap/Map.hpp>

int main()
{
	starmap::Map map;
	map.load(map_path);
	
	// Must load map before you can parse.
	map.parse();

	std::cin.get();
}
```

See [documentation](https://domre.github.io/starmap/) for more.  

### Notes  
Does *not* support external tilesets or objects.  
This library can throw C++ stanard exceptions.  

### Credits
* [JSON for C++](https://github.com/nlohmann/json).
* [zlib](https://zlib.net/).
* [zlib-win-build](https://github.com/kiyolee/zlib-win-build).
* [doxygen-dark-theme](https://github.com/MaJerle/doxygen-dark-theme).