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
	Terrain::Terrain()
	    : m_name {""}, m_tile {0}
	{
	}

	Terrain::Terrain(const nlohmann::json& json)
	    : m_name {""}, m_tile {0}
	{
		parse(json);
	}

	Terrain::~Terrain()
	{
		m_properties.clear();
	}

	void Terrain::parse(const nlohmann::json& json)
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
			auto prop_array = json.at("properties");
			for (const auto& prop : prop_array)
			{
				m_properties.emplace(prop.at("name"), prop);
			}
		}
	}

	std::string Terrain::get_name() const
	{
		return m_name;
	}

	const int Terrain::get_tile() const
	{
		return m_tile;
	}
} // namespace starmap