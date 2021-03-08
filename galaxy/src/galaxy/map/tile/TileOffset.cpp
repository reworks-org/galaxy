///
/// TileOffset.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "TileOffset.hpp"

namespace galaxy
{
	namespace map
	{
		TileOffset::TileOffset() noexcept
		    : m_x {0}, m_y {0}
		{
		}

		TileOffset::TileOffset(const nlohmann::json& json)
		    : m_x {0}, m_y {0}
		{
			parse(json);
		}

		void TileOffset::parse(const nlohmann::json& json)
		{
			if (json.count("x") > 0)
			{
				m_x = json.at("x");
			}

			if (json.count("y") > 0)
			{
				m_y = json.at("y");
			}
		}

		const int TileOffset::get_x() const noexcept
		{
			return m_x;
		}

		const int TileOffset::get_y() const noexcept
		{
			return m_y;
		}
	} // namespace map
} // namespace galaxy