///
/// Tileset.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "Tileset.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Tileset::Tileset() noexcept
		:m_backgroundColour("00FFFFFF"), m_columns(0), m_firstGID(0), m_grid(std::nullopt), m_image(""), m_imageHeight(0)
	{
	}

	Tileset::Tileset(const nlohmann::json& json)
	{
		parse(json);
	}

	Tileset::~Tileset() noexcept
	{
		m_grid.reset();
	}

	void Tileset::parse(const nlohmann::json& json)
	{
		if (json.count("backgroundcolor") > 0)
		{
			m_backgroundColour = json.at("backgroundcolor");
		}

		m_columns = json.at("columns");
		m_firstGID = json.at("firstgid");

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

		m_imageHeight = json.at("imageheight");
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
}