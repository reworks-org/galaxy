///
/// Map.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>
#include "starmap/layer/TileLayer.hpp"
#include "starmap/layer/ObjectLayer.hpp"
#include "starmap/layer/ImageLayer.hpp"
#include "starmap/layer/GroupLayer.hpp"

#include "Map.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Map::Map() noexcept
		:m_isLoaded(false), m_backgroundColour("00FFFFFF"), m_height(0), m_hexSideLength(0), m_infinite(false), m_nextLayerID(0), m_nextObjectID(0), m_orientation("orthogonal"), m_renderOrder("right-down"), m_staggerAxis(""), m_staggerIndex(""), m_tiledVersion(""), m_tileHeight(0), m_tileWidth(0), m_type("map"), m_width(0)
	{
	}

	Map::~Map() noexcept
	{
		m_root.clear();
		m_properties.clear();
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

			if (m_root.count("layers") > 0)
			{
				auto layerArray = m_root.at("layers");
				std::for_each(layerArray.begin(), layerArray.end(), [&](const nlohmann::json& layer)
				{
					std::string type = layer.at("type");
					if (type == "tilelayer")
					{
						m_layers.push_back(std::make_unique<starmap::TileLayer>());
					}
					else if (type == "objectgroup")
					{
						m_layers.push_back(std::make_unique<starmap::ObjectLayer>());
					}
					else if (type == "imagelayer ")
					{
						m_layers.push_back(std::make_unique<starmap::ImageLayer>());
					}
					else if (type == "group")
					{
						m_layers.push_back(std::make_unique<starmap::GroupLayer>());
					}
				});
			}
			
			m_nextLayerID = m_root.at("nextlayerid");
			m_nextObjectID = m_root.at("nextobjectid");
			m_orientation = m_root.at("orientation");

			if (m_root.count("properties") > 0)
			{
				auto propArray = m_root.at("properties");
				std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
				{
					m_properties.emplace(property.at("name"), property);
				});
			}

			m_renderOrder = m_root.at("renderorder");

			if (m_root.count("staggeraxis") > 0)
			{
				m_staggerAxis = m_root.at("staggeraxis");
			}

			if (m_root.count("staggerindex") > 0)
			{
				m_staggerIndex = m_root.at("staggerindex");
			}
			
			m_tiledVersion = m_root.at("tiledversion");
			m_tileHeight = m_root.at("tileheight");
			
			if (m_root.count("tilesets") > 0)
			{
				auto tsArray = m_root.at("tilesets");
				std::for_each(tsArray.begin(), tsArray.end(), [&](const nlohmann::json& tileset)
				{
					m_tileSets.emplace_back(tileset);
				});
			}

			m_tileWidth = m_root.at("tilewidth");
			
			if (m_root.count("type") > 0)
			{
				m_type = m_root.at("type");
			}
			
			m_width = m_root.at("width");
		}
	}

	const std::string& Map::getBackgroundColour() const noexcept
	{
		return m_backgroundColour;
	}

	const int Map::getHeight() const noexcept
	{
		return m_height;
	}

	const int Map::getHexSideLength() const noexcept
	{
		return m_hexSideLength;
	}

	const bool Map::isInfinite() const noexcept
	{
		return m_infinite;
	}

	const auto& Map::getLayers() const noexcept
	{
		return m_layers;
	}

	const int Map::getNextLayerID() const noexcept
	{
		return m_nextLayerID;
	}

	const int Map::getNextObjectID() const noexcept
	{
		return m_nextObjectID;
	}

	const std::string& Map::getOrientation() const noexcept
	{
		return m_orientation;
	}

	const std::string& Map::getRenderOrder() const noexcept
	{
		return m_renderOrder;
	}

	const std::string& Map::getStaggerAxis() const noexcept
	{
		return m_staggerAxis;
	}

	const std::string& Map::getStaggerIndex() const noexcept
	{
		return m_staggerIndex;
	}

	const std::string& Map::getTiledVersion() const noexcept
	{
		return m_tiledVersion;
	}

	const int Map::getTileHeight() const noexcept
	{
		return m_tileHeight;
	}

	const auto& Map::getTileSets() const noexcept
	{
		return m_tileSets;
	}

	const int Map::getTileWidth() const noexcept
	{
		return m_tileWidth;
	}

	const std::string& Map::getType() const noexcept
	{
		return m_type;
	}

	const int Map::getWidth() const noexcept
	{
		return m_width;
	}
}