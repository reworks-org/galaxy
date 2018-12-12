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
		:m_bounds(bounds), m_isVisible(true), m_tooltip(nullptr)
	{
	}

	Widget::~Widget()
	{
		m_tooltip.reset();
	}

	void Widget::setVisibility(const bool isVisible)
	{
		m_isVisible = isVisible;
	}

	const bool Widget::isVisible() const
	{
		return m_isVisible;
	}

	void Widget::setTooltip(std::unique_ptr<Tooltip> tooltip)
	{
		m_tooltip = std::move(tooltip);
	}
}