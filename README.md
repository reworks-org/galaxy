# starmap
[![Build Status](https://ci.appveyor.com/api/projects/status/yigrhkcvgofvhhwt?svg=true)](https://ci.appveyor.com/project/reworks/starmap)

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

## Docs


### Credits
* [JSON for C++](https://github.com/nlohmann/json).
* [zlib](https://zlib.net/).
* [zlib-win-build](https://github.com/kiyolee/zlib-win-build).