///
/// Widget.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	Tooltip* Widget::addTooltip(Theme* theme) noexcept
	{
		m_tooltip = std::make_unique<Tooltip>(theme);
		return m_tooltip.get();
	}

	const unsigned int Widget::id() const noexcept
	{
		return m_id;
	}

	Widget::Widget(galaxy::Theme* theme) noexcept
		:m_id(0), m_theme(theme), m_tooltip(nullptr)
	{
	}
}