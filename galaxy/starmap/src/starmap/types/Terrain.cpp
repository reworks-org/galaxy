///
/// Terrain.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Terrain.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Terrain::Terrain() noexcept
		:m_name(""), m_tile(0)
	{
	}

	Terrain::Terrain(const nlohmann::json& json) noexcept
		:m_name(""), m_tile(0)
	{
		parse(json);
	}

	Terrain::~Terrain() noexcept
	{
		m_properties.clear();
	}

	void Terrain::parse(const nlohmann::json& json) noexcept
	{
		if (json.count("name") > 0)
		{
			m_name = json.at("name");
		}

		if (json.count("tile") > 0)
		{
			m_tile = json.at("tile");
		}

		if (json.count("properties") > 0)
		{
			auto propArray = json.at("properties");
			std::for_each(propArray.begin(), propArray.end(), [&](const nlohmann::json& property)
			{
				m_properties.emplace(property.at("name"), property);
			});
		}
	}

	const std::string& Terrain::getName() const noexcept
	{
		return m_name;
	}

	const int Terrain::getTile() const noexcept
	{
		return m_tile;
	}
}