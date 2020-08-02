# starmap

C++20 JSON Tiled Map Parser.

## Supported
- Tiled 1.4.1.
- zlib compression.
- gzip compression.
- base64 compression.
- External tilesets in JSON format.

## Unsupported
- Zstandard compression.
- Object templates.

## Basic Usage
```cpp
starmap::Map map;
auto res = map.load("tiled/desert.tmx");
if (res)
{
	map.parse();
}
```