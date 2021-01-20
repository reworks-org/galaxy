///
/// WangTile.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangTile.hpp"

namespace galaxy
{
	namespace map
	{
		WangTile::WangTile()
		    : m_diagonal_flip {false}, m_horizontal_flip {false}, m_tile_id {0}, m_vertical_flip {false}
		{
		}

		WangTile::WangTile(const nlohmann::json& json)
		    : m_diagonal_flip {false}, m_horizontal_flip {false}, m_tile_id {0}, m_vertical_flip {false}
		{
			parse(json);
		}

		WangTile::~WangTile()
		{
			m_wang_indexes.clear();
		}

		void WangTile::parse(const nlohmann::json& json)
		{
			if (json.count("dflip") > 0)
			{
				m_diagonal_flip = json.at("dflip");
			}

			if (json.count("hflip") > 0)
			{
				m_horizontal_flip = json.at("hflip");
			}

			if (json.count("tileid") > 0)
			{
				m_tile_id = json.at("tileid");
			}

			if (json.count("vflip") > 0)
			{
				m_vertical_flip = json.at("vflip");
			}

			if (json.count("wangid") > 0)
			{
				auto id_array = json.at("wangid");
				for (const auto& wang_id : id_array)
				{
					m_wang_indexes.emplace_back(wang_id);
				}
			}
		}

		const bool WangTile::flipped_diagonally() const
		{
			return m_diagonal_flip;
		}

		const bool WangTile::flipped_horizontally() const
		{
			return m_horizontal_flip;
		}

		const int WangTile::get_tile_id() const
		{
			return m_tile_id;
		}

		const bool WangTile::flipped_vertically() const
		{
			return m_vertical_flip;
		}

		const auto& WangTile::get_indexes() const
		{
			return m_wang_indexes;
		}
	} // namespace map
} // namespace galaxy