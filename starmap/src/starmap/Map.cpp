///
/// Map.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include "nlohmann/json.hpp"
#include "nlohmann/detail/input/parser.hpp"

#include "Map.hpp"

using json = nlohmann::json;

///
/// Core namespace.
///
namespace starmap
{
	Map::Map() noexcept
	{
	}

	Map::~Map() noexcept
	{
	}

	bool Map::parse(std::string_view map)
	{
		bool result = true;
		std::filesystem::path path = map;

		json::parse

		return result;
	}

	bool Map::parse(void* data, const std::size_t size)
	{
		bool result = true;

		return result;
	}
}