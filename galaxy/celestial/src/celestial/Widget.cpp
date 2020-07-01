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
	const unsigned int Widget::id() const noexcept
	{
		return m_id;
	}

	Widget::Widget(celestial::Theme* theme) noexcept
		:m_theme(theme), m_id(0)
	{
	}
}