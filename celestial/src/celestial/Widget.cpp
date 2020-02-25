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
	Widget::Widget() noexcept
		:m_bounds(0, 0, 0, 0), m_theme(nullptr), m_drawTooltip(false), m_tooltip(nullptr), m_id(0)
	{
	}

	Widget::~Widget() noexcept
	{
		m_tooltip.reset();

		m_theme = nullptr;
		m_drawTooltip = false;
	}

	bool Widget::contains(const int x, const int y) noexcept
	{
		// Checks if the rectangle contains the point (x, y) using some basic math.
		return ((x > m_bounds.m_x) && (x < (m_bounds.m_x + m_bounds.m_width)) && (y > m_bounds.m_y) && (y < (m_bounds.m_y + m_bounds.m_height)));
	}

	Tooltip* Widget::createTooltip() noexcept
	{
		m_tooltip = std::make_unique<Tooltip>();
		m_tooltip->setTheme(m_theme);

		return m_tooltip.get();
	}

	void Widget::isVisible(const bool isVisible) noexcept
	{
		m_isVisible = isVisible;
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

	const protostar::Rect<int>& Widget::getBounds() const noexcept
	{
		return m_bounds;
	}

	const unsigned int Widget::id() const noexcept
	{
		return m_id;
	}
}