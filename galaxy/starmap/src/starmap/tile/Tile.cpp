///
/// Tile.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Tile.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Tile::Tile() noexcept
		:m_id(0), m_image(""), m_imageHeight(0), m_imageWidth(0), m_objectGroup(std::nullopt), m_probability(-1.0), m_type("")
	{
	}

	Tile::Tile(const nlohmann::json& json) noexcept
		:m_id(0), m_image(""), m_imageHeight(0), m_imageWidth(0), m_objectGroup(std::nullopt), m_probability(-1.0), m_type("")
	{
		parse(json);
	}

	Tile::~Tile() noexcept
	{
		m_animation.clear();
		m_properties.clear();
		m_terrainIndices.clear();
	}

	void Tile::parse(const nlohmann::json& json) noexcept
	{
		if (json.count("animation") > 0)
		{
			auto animations = json.at("animation");
			std::for_each(animations.begin(), animations.end(), [&](const nlohmann::json& animation)
			{
				m_animation.emplace_back(animation);
			});
		}

		if (json.count("id") > 0)
		{
			m_id = json.at("id");
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

		if (json.count("objectgroup") > 0)
		{
			auto objectGroup = json.at("objectgroup");
			m_objectGroup.emplace(objectGroup);
		}
		else
		{
			m_objectGroup = std::nullopt;
		}

		if (json.count("probability") > 0)
		{
			m_probability = json.at("probability");
		}

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}

		if (json.count("terrain") > 0)
		{
			auto terrainArray = json.at("terrain");
			std::for_each(terrainArray.begin(), terrainArray.end(), [&](const int terrain)
			{
				m_terrainIndices.emplace_back(terrain);
			});
		}

		if (json.count("type") > 0)
		{
			m_type = json.at("type");
		}
	}

	const auto& Tile::getAnimations() const noexcept
	{
		return m_animation;
	}

	const int Tile::getID() const noexcept
	{
		return m_id;
	}

	const std::string& Tile::getImage() const noexcept
	{
		return m_image;
	}

	const int Tile::getImageHeight() const noexcept
	{
		return m_imageHeight;
	}

	const int Tile::getImageWidth() const noexcept
	{
		return m_imageWidth;
	}

	const auto& Tile::getObjectGroup() const noexcept
	{
		return m_objectGroup;
	}

	const double Tile::getProbability() const noexcept
	{
		return m_probability;
	}

	const auto& Tile::getTerrainIndices() const noexcept
	{
		return m_terrainIndices;
	}

	const std::string& Tile::getType() const noexcept
	{
		return m_type;
	}
}