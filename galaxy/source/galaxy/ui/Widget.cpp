///
/// Widget.cpp
/// galaxy
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

galaxy
{
	Widget::Widget(const galaxyRect<int>& bounds, UITheme* theme)
		:m_bounds(bounds), m_theme(theme), m_isVisible(true), m_drawTooltip(false), m_tooltip(nullptr), m_id(0)
	{
	}

	Widget::~Widget()
	{
		m_tooltip.reset();
	}

	bool Widget::contains(const int x, const int y)
	{
		// If the mouse cursor is greater than the x axis but less than the total width of the button, and
		// Less than the height of the cursor, but greather than the y of the cursor take its height.
		return m_bounds.contains((int)x, (int)y);
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