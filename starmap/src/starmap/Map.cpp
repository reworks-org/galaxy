///
/// Map.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include "nlohmann/json.hpp"

#include "Map.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Map::Map() noexcept
		:m_isLoaded(false), m_backgroundColour("00FFFFFF"), m_height(0), m_hexSideLength(0), m_infinite(false)
	{
	}

	Map::~Map() noexcept
	{
		m_root.clear();
	}

	bool Map::load(const std::string& map)
	{
		bool result = true;

		// Makes sure the filepath is correct for the current platform.
		std::filesystem::path path = map;
		std::ifstream input(path.string(), std::ifstream::in);
		
		if (input.fail())
		{
			result = false;
		}
		else
		{
			// Use JSON stream to deserialize data and parse.
			input >> m_root;

			m_isLoaded = true;
		}

		return result;
	}

	bool Map::load(char* buffer, const std::size_t size)
	{
		bool result = true;

		// copy into string
		std::string strBuff(buffer, size);

		// Validate...
		if (!strBuff.data() || strBuff.empty())
		{
			result = false;
		}
		else
		{
			// This is the string parser.
			m_root = nlohmann::json::parse(strBuff);

			m_isLoaded = true;
		}

		return result;
	}

	void Map::parse()
	{
		// Make sure json is loaded to avoid error.
		if (m_isLoaded)
		{
			// Optional attribute.
			if (m_root.count("backgroundcolor") > 0)
			{
				m_backgroundColour = m_root.at("backgroundcolor");
			}

			m_height = m_root.at("height");

			// Only present on hexagonal maps.
			if (m_root.count("hexsidelength") > 0)
			{
				m_hexSideLength = m_root.at("hexsidelength");
			}
			
			m_infinite = m_root.at("infinite");

			auto layers = m_root.at("layers");
			//starmap::Layer layer;
			//layer.parse(layers);
		}
	}
}