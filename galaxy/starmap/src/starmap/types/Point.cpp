///
/// Point.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Point.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Point::Point()
	    : m_x {0.0}, m_y {0.0}
	{
	}

	Point::Point(const double x, const double y)
	    : m_x {x}, m_y {y}
	{
	}

	Point::Point(const nlohmann::json& json)
	    : m_x {0.0}, m_y {0.0}
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

	const double Point::get_x() const
	{
		return m_x;
	}

	const double Point::get_y() const
	{
		return m_y;
	}
} // namespace starmap