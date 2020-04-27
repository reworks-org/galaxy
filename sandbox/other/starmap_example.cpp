///
/// main.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <starmap/Map.hpp>

#include <iostream>

int main()
{
	try
	{
		starmap::Map map;
		map.load("../demo-maps/demo-zlib.json");
		map.parse();
		map.dump();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch (nlohmann::json::exception & e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}