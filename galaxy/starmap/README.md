# starmap

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

### Notes  
Does *not* support external tilesets or external objects.