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

	Widget::Widget() noexcept
		:Renderable(), Transform(), m_theme(nullptr), m_id(0)
	{
	}
}