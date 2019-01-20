///
/// TextInput.cpp
/// starlight
///
/// Created by reworks on 24/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro.h>

#include "sl/core/Keys.hpp"
#include "sl/utils/Time.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "TextInput.hpp"

namespace sl
{
	TextInput::TextInput(const sl::Rect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, UITheme* theme)
		:Widget(bounds, theme), m_drawIndicator(true), m_timePassed(0.0), m_isSelected(false), m_cursorPos(0), m_startPos(0), m_text(nullptr), m_field(nullptr), m_indicator(nullptr)
	{
		// Create textures
		m_field = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_field)
		{
			LOG_S(FATAL) << "Failed to create TextInput field bitmap.  Errno: " << al_get_errno();
		}

		m_indicator = al_create_bitmap(3, m_bounds.m_height - 8);
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to create TextInput indicator bitmap. Errno: " << al_get_errno();
		}

		// Create field bitmap.
		al_set_target_bitmap(m_field);
		al_clear_to_color(field);
		al_flip_display();

		// Create indicator bitmap.
		al_set_target_bitmap(m_indicator);
		al_clear_to_color(indicator);
		al_flip_display();
		
		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());

		// Prepare text.
		m_text = al_ustr_new("");

		m_textX = m_bounds.m_x + 2;
		m_textY = m_bounds.m_y + (static_cast<float>(m_bounds.m_height) / 2.0f);

		// Set up events.
		sl::Locator::dispatcher->sink<sl::KeyCharEvent>().connect<TextInput, &TextInput::receiveCharPress>(this);
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<TextInput, &TextInput::receiveMousePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<TextInput, &TextInput::receiveMouseMoved>(this);
	}

	TextInput::TextInput(const sl::Vector4<int>& vec4, const std::string& field, const std::string& indicator, UITheme* theme)
		:Widget({ vec4.m_w, vec4.m_x, 0, 0 }, theme), m_drawIndicator(true), m_timePassed(0.0), m_isSelected(false), m_cursorPos(0), m_startPos(0), m_text(nullptr), m_field(nullptr), m_indicator(nullptr), m_textX(vec4.m_y), m_textY(vec4.m_z)
	{
		// Load and validate field texture.
		m_field = m_theme->widgetTexture(field);
		if (!m_field)
		{
			LOG_S(FATAL) << "Failed to create TextInput field sub bitmap: " << field << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_field);
			m_bounds.m_height = al_get_bitmap_height(m_field);
		}

		// Load and validate indicator texture.
		m_indicator = m_theme->widgetTexture(indicator);
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to create TextInput indicator sub bitmap: " << indicator << " Errno: " << al_get_errno();
		}

		// Set up events.
		sl::Locator::dispatcher->sink<sl::KeyCharEvent>().connect<TextInput, &TextInput::receiveCharPress>(this);
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<TextInput, &TextInput::receiveMousePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<TextInput, &TextInput::receiveMouseMoved>(this);
	}

	TextInput::TextInput(const sol::table& table, UITheme* theme)
		:Widget({0, 0, 0, 0}, theme), m_drawIndicator(true), m_timePassed(0.0), m_isSelected(false), m_cursorPos(0), m_startPos(0), m_text(nullptr), m_field(nullptr), m_indicator(nullptr)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Text pos
		m_textX = table.get<int>("textX");
		m_textY = table.get<int>("textY");

		// Load and validate field texture.
		m_field = m_theme->widgetTexture(table.get<std::string>("field"));
		if (!m_field)
		{
			LOG_S(FATAL) << "Failed to create TextInput field sub bitmap: " << table.get<std::string>("field") << " Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_field);
			m_bounds.m_height = al_get_bitmap_height(m_field);
		}

		// Load and validate indicator texture.
		m_indicator = m_theme->widgetTexture(table.get<std::string>("indicator"));
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to create TextInput indicator sub bitmap: " << table.get<std::string>("indicator") << " Errno: " << al_get_errno();
		}

		// Set up events.
		sl::Locator::dispatcher->sink<sl::KeyCharEvent>().connect<TextInput, &TextInput::receiveCharPress>(this);
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<TextInput, &TextInput::receiveMousePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<TextInput, &TextInput::receiveMouseMoved>(this);
	}
	
	TextInput::~TextInput() noexcept
	{
		// Free everything.
		
		if (m_text)
		{
			al_ustr_free(m_text);
		}

		if (m_field)
		{
			al_destroy_bitmap(m_field);
		}

		if (m_indicator)
		{
			al_destroy_bitmap(m_indicator);
		}
	}

	void TextInput::receiveMouseMoved(const sl::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_drawTooltip = true;
			}
			else
			{
				m_drawTooltip = false;
			}
		}
	}

	void TextInput::receiveMousePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && e.m_button == 1)
			{
				m_isSelected = true;
			}
			else
			{
				m_isSelected = false;
			}
		}
	}

	void TextInput::receiveCharPress(const sl::KeyCharEvent& e)
	{
		if (m_isVisible && m_isSelected)
		{
			if (e.m_keycode == sl::Keys::UI_CONFIRM)
			{
				m_isSelected = false;
			}
			else
			{
				// Manipulate the string depending on the input type.
				switch (e.m_keycode)
				{
				case ALLEGRO_KEY_HOME:
					m_cursorPos = 0;
					break;

				case ALLEGRO_KEY_END:
					m_cursorPos = static_cast<int>(al_ustr_size(m_text));
					break;

				case ALLEGRO_KEY_LEFT:
					al_ustr_prev(m_text, &m_cursorPos);
					break;

				case ALLEGRO_KEY_RIGHT:
					al_ustr_next(m_text, &m_cursorPos);
					break;

				case ALLEGRO_KEY_BACKSPACE:
					if (al_ustr_prev(m_text, &m_cursorPos) == true)
					{
						al_ustr_remove_chr(m_text, m_cursorPos);
						al_ustr_prev(m_text, &m_startPos);
					}

				case ALLEGRO_KEY_DELETE:
					al_ustr_remove_chr(m_text, m_cursorPos);
					break;

				default:
					if (e.m_unichar >= ' ')
					{
						al_ustr_insert_chr(m_text, m_cursorPos, e.m_unichar);
						m_cursorPos += static_cast<int>(al_utf8_width(e.m_unichar));
					}
					break;
				}
			}

			// This ensures that the text does not go outside the input boundaries.
			// By ensuring the shown text is the latest character, minus early characters to give the impression
			// of scrolling text box.
			int usw = static_cast<int>(al_get_ustr_width(m_theme->font(), al_ref_ustr(&m_info, m_text, m_startPos, static_cast<int>(al_ustr_size(m_text)))));
			if (usw >= (m_bounds.m_width - 2))
			{
				al_ustr_next(m_text, &m_startPos);
			}
		}
	}

	void TextInput::update(const double dt)
	{
		if (m_isVisible)
		{
			if (m_isSelected)
			{
				// This ensures that the text does not go outside the input boundaries.
				// By ensuring the shown text is the latest character, minus early characters to give the impression
				// of scrolling text box.
				int usw = static_cast<int>(al_get_ustr_width(m_theme->font(), al_ref_ustr(&m_info, m_text, m_startPos, static_cast<int>(al_ustr_size(m_text)))));
				if (usw >= (m_bounds.m_width - 2))
				{
					al_ustr_next(m_text, &m_startPos);
				}
			}

			m_timePassed += dt;
			if (m_timePassed >= Time::milliseconds(700))
			{
				m_timePassed = 0.0;
				(m_drawIndicator == true) ? m_drawIndicator = false : m_drawIndicator = true;
			}
		}
	}

	void TextInput::render()
	{
		if (m_isVisible)
		{
			// Draw field and indicator.
			if (m_isSelected)
			{
				al_draw_bitmap(m_field, m_bounds.m_x, m_bounds.m_y, 0);
				
				if (m_drawIndicator)
				{
					// Get positon of indicator to draw, by getting pos of character to type.
					int after = m_cursorPos;
					al_ustr_next(m_text, &after);

					// Draw at x pos, which is the widget field x pos + the offset of the text + 1 character to mark the pos of the next character to type.
					// Draw at y centered on field.
					al_draw_bitmap(m_indicator, 
						m_bounds.m_x + al_get_ustr_width(m_theme->font(), al_ref_ustr(&m_info, m_text, m_startPos, after)), // x
						static_cast<float>(m_bounds.m_y) - (static_cast<float>(al_get_bitmap_height(m_indicator)) / 2.0f) + (static_cast<float>(al_get_bitmap_height(m_field)) / 2.0f),  // y
					0);
				}

				al_draw_ustr(m_theme->font(), m_theme->colour(), m_textX, m_textY, 0, al_ref_ustr(&m_info, m_text, m_startPos, static_cast<int>(al_ustr_size(m_text))));
			}
			else
			{
				al_draw_bitmap(m_field, m_bounds.m_x, m_bounds.m_y, 0);
				al_draw_ustr(m_theme->font(), m_theme->colour(), m_textX, m_textY, 0, al_ref_ustr(&m_info, m_text, m_startPos, static_cast<int>(al_ustr_size(m_text))));
			}

			// Draw tooltip.
			if (m_tooltip && m_drawTooltip)
			{
				m_tooltip->draw();
			}
		}
	}

	void TextInput::stopInput()
	{
		m_isSelected = false;
	}

	std::string TextInput::getText()
	{
		return std::string(al_cstr(m_text));
	}

	void TextInput::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}