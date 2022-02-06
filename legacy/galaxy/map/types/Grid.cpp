///
/// Grid.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Grid.hpp"

namespace galaxy
{
	namespace map
	{
		Grid::Grid() noexcept
			: m_height {0}
			, m_orientation {"orthogonal"}
			, m_width {0}
		{
		}

		Grid::Grid(const nlohmann::json& json)
			: m_height {0}
			, m_orientation {"orthogonal"}
			, m_width {0}
		{
			parse(json);
		}

		void Grid::parse(const nlohmann::json& json)
		{
			if (json.count("height") > 0)
			{
				m_height = json.at("height");
			}

			if (json.count("orientation") > 0)
			{
				m_orientation = json.at("orientation");
			}

			if (json.count("width") > 0)
			{
				m_width = json.at("width");
			}
		}

		const int Grid::get_height() const noexcept
		{
			return m_height;
		}

		const std::string& Grid::get_orientation() const noexcept
		{
			return m_orientation;
		}

		const int Grid::get_width() const noexcept
		{
			return m_width;
		}
	} // namespace map
} // namespace galaxy