///
/// Tooltip.cpp
/// starlight
///
/// Created by reworks on 12/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/ui/UITheme.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Tooltip.hpp"

namespace sl
{
	Tooltip::Tooltip(const std::string& text, const std::string& texture, const sl::Vector2<int>& section, UITheme* theme)
		:m_x(0), m_y(0), m_text(nullptr), m_theme(theme)
	{
		// Need to calculate new bitmap dimensions, render and repeat section to get a proper box, then render text ontop.
		ALLEGRO_BITMAP* sub = m_theme->widgetTexture(texture);
		if (!sub)
		{
			LOG_S(FATAL) << "Failed to retrieve sub texture from master texture: " << texture << " Errno: " << al_get_errno();
		}
		else
		{
			float width = al_get_text_width(m_theme->font(), text.c_str());
			float sectionWidth = section.m_y - section.m_x;
			int repeat = std::ceil(width / sectionWidth);
			int textureHeight = al_get_bitmap_height(sub);

			// Create text bitmap, with the same height, and calculate the total width, which is the total sections plus each side of the master
			// texture, subtracting the extra section from the master bitmap.
			m_text = al_create_bitmap((repeat * sectionWidth) + (al_get_bitmap_width(sub) - sectionWidth), textureHeight);
			al_set_target_bitmap(m_text);
			al_clear_to_color(al_map_rgba(255, 255, 255, 0));

			// Draw opening part of tooltip. Take a pixel to make sure we dont draw the first pixel of the next section.
			al_draw_bitmap_region(sub, 0, 0, section.m_x - 1, textureHeight, 0, 0, 0);

			// Now draw that texture for 'repeat' amount of times to fill out tooltip graphic.
			int nextSectionX = section.m_x;
			for (int i = 0; i < repeat; ++i)
			{
				al_draw_bitmap_region(sub, section.m_x, 0, sectionWidth, textureHeight, nextSectionX, 0, 0);
				nextSectionX += sectionWidth;
			}

			// Draw ending of tooltip. Add a pixel to make sure we dont draw the last pixel of the previous section.
			al_draw_bitmap_region(sub, section.m_y + 1, 0, (al_get_bitmap_width(sub) - sectionWidth) - (section.m_x - 1), textureHeight, nextSectionX, 0, 0);

			// Now draw the text to the tooltip graphic.
			al_draw_text(m_theme->font(), m_theme->colour(), al_get_bitmap_width(m_text) / 2.0f, al_get_bitmap_height(m_text) / 2.0f, ALLEGRO_ALIGN_CENTER, text.c_str());
			
			al_flip_display();
			al_set_target_backbuffer(Locator::window->getDisplay());

			// Cleanup since we are done with this texture.
			al_destroy_bitmap(sub);
		}

		// Register the recieve() method with entt to mouse events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect(this);
	}

	Tooltip::Tooltip(const std::string& text, UITheme* theme)
		:m_x(0), m_y(0), m_text(nullptr), m_theme(theme)
	{
		// Create bitmap with text dimensions to, then draw text. The usual.
		m_text = al_create_bitmap(al_get_text_width(m_theme->font(), text.c_str()), al_get_font_line_height(m_theme->font()));

		al_set_target_bitmap(m_text);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));

		al_draw_text(m_theme->font(), m_theme->colour(), 0, 0, 0, text.c_str());
		
		al_flip_display();
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Register the recieve() method with entt to mouse events.
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect(this);
	}

	Tooltip::~Tooltip() noexcept
	{
		if (m_text)
		{
			al_destroy_bitmap(m_text);
		}
	}

	void Tooltip::receive(const sl::MouseMovedEvent& e)
	{
		m_x = e.m_x + 12;
		m_y = e.m_y + 12;
	}

	void Tooltip::draw()
	{
		al_draw_bitmap(m_text, m_x, m_y, 0);
	}
}