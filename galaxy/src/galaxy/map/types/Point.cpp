///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Point.hpp"

namespace galaxy
{
	namespace map
	{
		Point::Point() noexcept
			: m_x {0.0}
			, m_y {0.0}
		{
		}

		Point::Point(const double x, const double y) noexcept
			: m_x {x}
			, m_y {y}
		{
		}

		Point::Point(const nlohmann::json& json)
			: m_x {0.0}
			, m_y {0.0}
		{
			parse(json);
		}

		void Point::parse(const nlohmann::json& json)
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

		const double Point::get_x() const noexcept
		{
			return m_x;
		}

		const double Point::get_y() const noexcept
		{
			return m_y;
		}
	} // namespace map
} // namespace galaxy