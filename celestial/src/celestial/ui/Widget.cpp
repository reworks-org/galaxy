///
/// Widget.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

namespace celestial
{
	Widget::Widget(const protostar::Rect<int>& bounds, UITheme* theme)
		:m_bounds(bounds), m_theme(theme), m_isVisible(true), m_drawTooltip(false), m_tooltip(nullptr), m_id(0)
	{
	}

	Widget::~Widget()
	{
		m_tooltip.reset();
	}

	bool Widget::contains(const int x, const int y)
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		bool out = ((x > m_bounds.m_x) && (x < (m_bounds.m_x + m_bounds.m_w)) && (y > m_bounds.m_y) && (y < (m_bounds.m_y + m_bounds.m_h))) ? true : false;

		return out;
	}

	void Widget::setVisibility(const bool isVisible)
	{
		m_isVisible = isVisible;
	}

	const bool Widget::isVisible() const
	{
		return m_isVisible;
	}

	const unsigned int Widget::id() const
	{
		return m_id;
	}
}