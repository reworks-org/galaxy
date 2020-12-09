///
/// Circle.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Circle.hpp"

///
/// Core namespace.
///
namespace pr
{
	Circle::Circle()
	    : m_pos {0.0f, 0.0f}, m_radius {0.0f}
	{
	}

	Circle::Circle(const float x, const float y, const float radius)
	    : m_pos {x, y}, m_radius {radius}
	{
	}

	void Circle::create(const float x, const float y, const float radius)
	{
		m_pos    = {x, y};
		m_radius = radius;
	}

	const float Circle::get_x() const
	{
		return m_pos.x;
	}

	const float Circle::get_y() const
	{
		return m_pos.y;
	}

	const glm::vec2& Circle::get_xy() const
	{
		return m_pos;
	}

	const float Circle::radius() const
	{
		return m_radius;
	}
} // namespace pr