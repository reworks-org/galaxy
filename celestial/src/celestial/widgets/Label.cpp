///
/// Label.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Label.hpp"

namespace celestial
{
	Label::Label(const int x, const int y, const std::string& text, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_text(text)
	{
	}

	Label::~Label()
	{
	}

	void Label::update(const double dt)
	{
	}

	void Label::render(celestial::compat::Renderer* renderer)
	{
		if (m_isVisible)
		{
			renderer->drawText(m_theme->font(), m_theme->colour(), m_text, m_bounds.m_x, m_bounds.m_y);
		}
	}

	void Label::setText(const std::string& text)
	{
		m_text = text;
	}

	void Label::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}