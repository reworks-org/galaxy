///
/// WangSet.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangSet.hpp"

namespace galaxy
{
	namespace map
	{
		WangSet::WangSet() noexcept
		    : m_name {""}, m_tile_id {0}
		{
		}

		WangSet::WangSet(const nlohmann::json& json)
		    : m_name {""}, m_tile_id {0}
		{
			parse(json);
		}

		WangSet::~WangSet() noexcept
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
				const auto& corner_array = json.at("cornercolors");
				for (auto& corner : corner_array)
				{
					m_corner_colours.emplace_back(corner);
				}
			}

			if (json.count("edgecolors") > 0)
			{
				const auto& edge_array = json.at("edgecolors");
				for (auto& edge : edge_array)
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
				const auto& prop_array = json.at("properties");
				for (auto& prop : prop_array)
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
				const auto& tile_array = json.at("wangtiles");
				for (auto& tile : tile_array)
				{
					m_tiles.emplace_back(tile);
				}
			}
		}

		const std::vector<WangColour>& WangSet::get_corner_colours() const noexcept
		{
			return m_corner_colours;
		}

		const std::vector<WangTile>& WangSet::get_edge_colours() const noexcept
		{
			return m_edge_colours;
		}

		const std::string& WangSet::get_name() const noexcept
		{
			return m_name;
		}

		const int WangSet::get_tile_id() const noexcept
		{
			return m_tile_id;
		}

		const std::vector<WangTile>& WangSet::get_tiles() const noexcept
		{
			return m_tiles;
		}
	} // namespace map
} // namespace galaxy