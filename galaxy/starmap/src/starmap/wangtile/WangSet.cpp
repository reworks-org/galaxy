///
/// WangSet.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangSet.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangSet::WangSet()
	    : m_name {""}, m_tile_id {0}
	{
	}

	WangSet::WangSet(const nlohmann::json& json)
	    : m_name {""}, m_tile_id {0}
	{
		parse(json);
	}

	WangSet::~WangSet()
	{
		m_corner_colours.clear();
		m_edge_colours.clear();
		m_properties.clear();
		m_tiles.clear();
	}

	void WangSet::parse(const nlohmann::json& json)
	{
		if (json.count("cornercolors") > 0)
		{
			auto corner_array = json.at("cornercolors");
			for (const auto& corner : corner_array)
			{
				m_corner_colours.emplace_back(corner);
			}
		}

		if (json.count("edgecolors") > 0)
		{
			auto edge_array = json.at("edgecolors");
			for (const auto& edge : edge_array)
			{
				m_edge_colours.emplace_back(edge);
			}
		}

		if (json.count("name") > 0)
		{
			m_name = json.at("name");
		}

		if (json.count("properties") > 0)
		{
			auto prop_array = json.at("properties");
			for (const auto& prop : prop_array)
			{
				m_properties.emplace(prop.at("name"), prop);
			}
		}

		if (json.count("tile") > 0)
		{
			m_tile_id = json.at("tile");
		}

		if (json.count("wangtiles") > 0)
		{
			auto tile_array = json.at("wangtiles");
			for (const auto& tile : tile_array)
			{
				m_tiles.emplace_back(tile);
			}
		}
	}

	const auto& WangSet::get_corner_colours() const
	{
		return m_corner_colours;
	}

	const auto& WangSet::get_edge_colours() const
	{
		return m_edge_colours;
	}

	std::string WangSet::get_name() const
	{
		return m_name;
	}

	const int WangSet::get_tile_id() const
	{
		return m_tile_id;
	}

	const auto& WangSet::get_tiles() const
	{
		return m_tiles;
	}
} // namespace starmap