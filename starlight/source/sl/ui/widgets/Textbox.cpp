///
/// Textbox.cpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/utils/Time.hpp"
#include "sl/events/Keys.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Textbox.hpp"

namespace sl
{
	Textbox::Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const std::string& font, const ALLEGRO_COLOR col, const unsigned int duration, const std::string& speaker)
		:Widget({x, y, 0, 0}), m_frame(nullptr), m_indicator(nullptr), m_messages(messages), m_speaker(speaker), m_font(nullptr), m_colour(col), m_duration(duration), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_index(0), m_drawLoweredIndicator(false)
	{
		m_frame = al_load_bitmap(frame.c_str());
		if (!m_frame)
		{
			LOG_S(FATAL) << "Failed to load texture: " << frame << "Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_frame);
			m_bounds.m_height = al_get_bitmap_height(m_frame);
		}

		m_indicator = al_load_bitmap(indicator.c_str());
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to load texture: " << indicator << "Errno: " << al_get_errno();
		}

		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to retrieve font: " << font << "Errno: " << al_get_errno();
		}

		Locator::dispatcher->sink<sl::KeyDownEvent>().connect(this);
	}

	Textbox::~Textbox()
	{
		if (m_frame)
		{
			al_destroy_bitmap(m_frame);
		} 

		if (m_indicator)
		{
			al_destroy_bitmap(m_indicator);
		}

		m_messages.clear();
	}

	void Textbox::receive(const sl::KeyDownEvent& e)
	{
		if (e.m_keycode == sl::Keys::TEXTBOX_CONFIRM)
		{
			++m_index;
			if (m_index > (m_messages.size() - 1))
			{
				m_index = (m_messages.size() - 1);
			}

			m_characterIndex = 0;
			m_timePassed = 0.0;
		}
	}

	void Textbox::update(const double dt)
	{
		if (m_isVisible && m_duration > 0)
		{
			// Update time passed and check if its exceeded the duration allowed for each character.
			m_indicatorTimePassed += dt;
			if (m_indicatorTimePassed > Time::milliseconds(500))
			{
				(m_drawLoweredIndicator == true) ? m_drawLoweredIndicator = false : m_drawLoweredIndicator = true;

				m_indicatorTimePassed = 0.0;
			}
			
			m_timePassed += dt;
			if (m_timePassed > Time::milliseconds(m_duration))
			{
				if (!((m_characterIndex > (m_messages[m_index].size() - 1))))
				{
					++m_characterIndex;
				}

				m_timePassed = 0.0;
			}
		}
	}

	void Textbox::render()
	{
		if (m_isVisible)
		{
			// Draw frame on the bottom.
			al_draw_bitmap(m_frame, m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);

			// Then if there is a speaker, add that.
			if (!m_speaker.empty())
			{
				al_draw_text(m_font, m_colour, m_bounds.m_x + m_offsetX + 2, m_bounds.m_y + m_offsetY + 2, 0, m_speaker.c_str());
			}
			
			// Then draw the relevant text.
			al_draw_text(m_font, m_colour, m_bounds.m_x + m_offsetX + 2, m_bounds.m_y + m_offsetY + al_get_font_line_height(m_font), 0, m_messages[m_index].substr(0, m_characterIndex).c_str());

			// Then draw the indicator finally.
			float x = m_bounds.m_x + m_offsetX + m_bounds.m_width - al_get_bitmap_width(m_indicator) - 2;
			float y = m_bounds.m_y + m_offsetY + m_bounds.m_height - al_get_bitmap_height(m_indicator) - 2;
			if (m_drawLoweredIndicator)
			{
				y += 2;
			}
			
			al_draw_bitmap(m_indicator, x, y, 0);
		}
	}
}