///
/// Tooltip.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Tooltip.hpp"

///
/// Core namespace.
///
namespace celestial
{
	void Tooltip::setTheme(celestial::UITheme* theme) noexcept
	{
		m_theme = theme;
	}

	void Tooltip::setText(const std::string& text) noexcept
	{
		m_text = text;
	}
}