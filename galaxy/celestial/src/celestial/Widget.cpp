///
/// Widget.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Widget::~Widget() noexcept
	{
	}

	bool Widget::contains(const int x, const int y) noexcept
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		return ((x > m_bounds.m_x) && (x < (m_bounds.m_x + m_bounds.m_width)) && (y > m_bounds.m_y) && (y < (m_bounds.m_y + m_bounds.m_height)));
	}

	void Widget::setVisibility(const bool isVisible) noexcept
	{
		m_visible.set(isVisible);
	}

	const unsigned int Widget::id() const noexcept
	{
		return m_id;
	}

	Widget::Widget() noexcept
		:m_visible(true), m_theme(nullptr), m_id(0)
	{
	}

	void Widget::setTheme(celestial::UITheme* theme) noexcept
	{
		m_theme = theme;
	}

	void Widget::setBounds(const int x, const int y, const int w, const int h) noexcept
	{
		m_bounds.m_x = x;
		m_bounds.m_y = y;
		m_bounds.m_width = w;
		m_bounds.m_height = h;
	}
}