///
/// Label.cpp
/// starlight
///
/// Created by reworks on 17/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "Label.hpp"

namespace sl
{
	Label::Label(const int x, const int y, const std::string& text, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_text(text)
	{
	}

	Label::Label(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get text data.
		m_text = table.get<std::string>("text");
	}

	Label::~Label()
	{
	}

	void Label::update(const double dt)
	{
	}

	void Label::render()
	{
		if (m_isVisible)
		{
			al_draw_text(m_theme->font(), m_theme->colour(), m_bounds.m_x, m_bounds.m_y, 0, m_text.c_str());
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