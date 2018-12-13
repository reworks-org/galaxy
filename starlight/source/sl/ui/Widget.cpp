///
/// Widget.cpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

namespace sl
{
	Widget::Widget(const sl::Rect<int>& bounds) noexcept
		:m_bounds(bounds), m_isVisible(true), m_tooltip(nullptr), m_offsetX(0), m_offsetY(0)
	{
	}

	Widget::~Widget()
	{
		m_tooltip.reset();
	}

	void Widget::setOffset(const int x, const int y)
	{
		m_offsetX = x;
		m_offsetY = y;
	}

	void Widget::setVisibility(const bool isVisible)
	{
		m_isVisible = isVisible;
	}

	const bool Widget::isVisible() const
	{
		return m_isVisible;
	}
}