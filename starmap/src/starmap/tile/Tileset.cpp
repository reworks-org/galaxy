///
/// Tileset.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Tileset.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Tileset::Tileset() noexcept
		:m_backgroundColour("00FFFFFF"), m_columns(0), m_firstGID(0), m_grid(std::nullopt), m_image(""), m_imageHeight(0), m_imageWidth(0), m_margin(0), m_name(""), m_source(""), m_spacing(0), m_tileCount(0), m_tiledVersion(""), m_tileHeight(0), m_tileOffset(std::nullopt), m_tileWidth(0), m_transparentColour("FFFFFF"), m_type("tileset")
	{
	}

	Tileset::Tileset(const nlohmann::json& json)
		:m_backgroundColour("00FFFFFF"), m_columns(0), m_firstGID(0), m_grid(std::nullopt), m_image(""), m_imageHeight(0), m_imageWidth(0), m_margin(0), m_name(""), m_source(""), m_spacing(0), m_tileCount(0), m_tiledVersion(""), m_tileHeight(0), m_tileOffset(std::nullopt), m_tileWidth(0), m_transparentColour("FFFFFF"), m_type("tileset")
	{
		parse(json);
	}

	Tileset::~Tileset() noexcept
	{
		m_grid.reset();
		m_properties.clear();
		m_terrain.clear();
		m_tiles.clear();
		m_wangSets.clear();
	}

	void Tileset::parse(const nlohmann::json& json)
	{
		if (json.count("backgroundcolor") > 0)
		{
			m_backgroundColour = json.at("backgroundcolor");
		}

		if (json.count("columns") > 0)
		{
			m_columns = json.at("columns");
		}

		if (json.count("firstgid") > 0)
		{
			m_firstGID = json.at("firstgid");
		}

		if (json.count("grid") > 0)
		{
			auto gridArray = json.at("grid");
			m_grid.emplace(gridArray);
		}
		else
		{
			m_grid = std::nullopt;
		}

		if (json.count("image") > 0)
		{
			m_image = json.at("image");
		}
		
		if (json.count("imageheight") > 0)
		{
			m_imageHeight = json.at("imageheight");
		}
		
		if (json.count("imagewidth") > 0)
		{
			m_imageWidth = json.at("imagewidth");
		}
		
		if (json.count("margin") > 0)
		{
			m_margin = json.at("margin");
		}

		if (json.count("name") > 0)
		{
			m_name = json.at("name");
		}

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}

		if (json.count("source") > 0)
		{
			m_source = json.at("source");
		}
		
		if (json.count("spacing") > 0)
		{
			m_spacing = json.at("spacing");
		}

		if (json.count("terrains") > 0)
		{
			auto terrainArray = json.at("terrains");
			std::for_each(terrainArray.begin(), terrainArray.end(), [&](const nlohmann::json& terrain)
			{
				m_terrain.emplace_back(terrain);
			});
		}
		
		if (json.count("tilecount") > 0)
		{
			m_tileCount = json.at("tilecount");
		}

		if (json.count("tiledversion") > 0)
		{
			m_tiledVersion = json.at("tiledversion");
		}

		if (json.count("tileheight") > 0)
		{
			m_tileHeight = json.at("tileheight");
		}

		if (json.count("tileoffset") > 0)
		{
			auto tileOffset = json.at("tileoffset");
			m_tileOffset.emplace(tileOffset);
		}
		else
		{
			m_grid = std::nullopt;
		}

		if (json.count("tiles") > 0)
		{
			auto tileArray = json.at("tiles");
			std::for_each(tileArray.begin(), tileArray.end(), [&](const nlohmann::json& tiles)
			{
				m_tiles.emplace_back(tiles);
			});
		}

		if (json.count("tilewidth") > 0)
		{
			m_tileWidth = json.at("tilewidth");
		}

		if (json.count("transparentcolor") > 0)
		{
			m_transparentColour = json.at("transparentcolor");
		}
		
		if (json.count("type") > 0)
		{
			m_type = json.at("type");
		}

		if (json.count("wangsets") > 0)
		{
			auto wangSets = json.at("wangsets");
			std::for_each(wangSets.begin(), wangSets.end(), [&](const nlohmann::json& wangset)
			{
				m_wangSets.emplace_back(wangSets);
			});
		}
	}

	const std::string& Tileset::getBackgroundColour() const noexcept
	{
		return m_backgroundColour;
	}

	const int Tileset::getColumns() const noexcept
	{
		return m_columns;
	}

	const int Tileset::getFirstGID() const noexcept
	{
		return m_firstGID;
	}

	const auto& Tileset::getGrid() const noexcept
	{
		return m_grid;
	}

	const std::string& Tileset::getImage() const noexcept
	{
		return m_image;
	}

	const int Tileset::getImageHeight() const noexcept
	{
		return m_imageHeight;
	}

	const int Tileset::getImageWidth() const noexcept
	{
		return m_imageWidth;
	}

	const int Tileset::getMargin() const noexcept
	{
		return m_margin;
	}

	const std::string& Tileset::getName() const noexcept
	{
		return m_name;
	}

	const auto& Tileset::getTerrain() const noexcept
	{
		return m_terrain;
	}

	const int Tileset::getTileCount() const noexcept
	{
		return m_tileCount;
	}

	const std::string& Tileset::getTiledVersion() const noexcept
	{
		return m_tiledVersion;
	}

	const int Tileset::getTileHeight() const noexcept
	{
		return m_tileHeight;
	}

	const auto& Tileset::getTileOffset() const noexcept
	{
		return m_tileOffset;
	}
	
	const auto& Tileset::getTiles() const noexcept
	{
		return m_tiles;
	}

	const int Tileset::getTileWidth() const noexcept
	{
		return m_tileWidth;
	}

	const std::string& Tileset::getTransparentColour() const noexcept
	{
		return m_transparentColour;
	}

	const std::string& Tileset::getType() const noexcept
	{
		return m_type;
	}

	const auto& Tileset::getWangSets() const noexcept
	{
		return m_wangSets;
	}
}