///
/// Tooltip.cpp
/// starlight
///
/// Created by reworks on 12/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/ui/UITheme.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Tooltip.hpp"

namespace sl
{
	Tooltip::Tooltip(const std::string& text, UITheme* theme, const float maxWidth)
		:m_text(text), m_maxWidth(maxWidth), m_lineHeight(0), m_theme(theme)
	{
		// Calculate and store line height to avoid doing this every draw call.
		m_lineHeight = al_get_font_line_height(m_theme->font());

		// Register the recieve() method with entt to mouse events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect(this);
	}

	void Tooltip::receive(const sl::MouseMovedEvent& e)
	{
		m_x = e.m_x + 12;
		m_y = e.m_y + 12;
	}

	void Tooltip::draw()
	{
		al_draw_multiline_text(m_theme->font(), m_theme->colour(), m_x, m_y, m_maxWidth, m_lineHeight, ALLEGRO_ALIGN_LEFT, m_text.c_str());
	}
}