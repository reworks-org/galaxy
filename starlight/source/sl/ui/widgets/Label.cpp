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
	Label::Label(const int x, const int y, const std::string& text, const std::string& font, const ALLEGRO_COLOR col)
		:Widget({ x, y, 0, 0 }), m_text(text), m_colour(col)
	{
		// Create bitmap, draw text to it, then store it.
		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Could not retrieve font: " << font;
		}
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
			al_draw_text(m_font, m_colour, m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, ALLEGRO_ALIGN_LEFT, m_text.c_str());
		}
	}

	void Label::setText(const std::string& text)
	{
		m_text = text;
	}
}