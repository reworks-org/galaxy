///
/// Textbox.cpp
/// starlight
///
/// Created by reworks on 21/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/Keys.hpp"
#include "sl/utils/Time.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Textbox.hpp"

namespace sl
{
	Textbox::Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const unsigned int maxWidth, const unsigned int duration, UITheme* theme, const std::string& speaker)
		:Widget({x, y, 0, 0}, theme), m_frame(nullptr), m_indicator(nullptr), m_messages(messages), m_speaker(speaker), m_maxWidth(maxWidth), m_duration(duration), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_page(0), m_drawLoweredIndicator(false), m_lineHeight(0)
	{
		// Load frame texture.
		m_frame = m_theme->widgetTexture(frame);
		if (!m_frame)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << frame << "Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_frame);
			m_bounds.m_height = al_get_bitmap_height(m_frame);
		}

		// Load indicator texture.
		m_indicator = m_theme->widgetTexture(indicator);
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << indicator << "Errno: " << al_get_errno();
		}

		m_lineHeight = al_get_font_line_height(m_theme->font());

		// Connects receive() method automatically.
		Locator::dispatcher->sink<sl::KeyDownEvent>().connect(this);
	}

	Textbox::Textbox(const sol::table& table, UITheme* theme)
		:Widget({ 0, 0, 0, 0 }, theme), m_frame(nullptr), m_indicator(nullptr), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_page(0), m_drawLoweredIndicator(false), m_lineHeight(0)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get textbox data.
		m_maxWidth = table.get<unsigned int>("maxWidth");
		m_duration = table.get<unsigned int>("duration");
		m_speaker = table.get<std::string>("speaker");
		
		// Load frame texture.
		m_frame = m_theme->widgetTexture(table.get<std::string>("frame"));
		if (!m_frame)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << table.get<std::string>("frame") << "Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_frame);
			m_bounds.m_height = al_get_bitmap_height(m_frame);
		}

		// Load indicator texture.
		m_indicator = m_theme->widgetTexture(table.get<std::string>("indicator"));
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to create sub bitmap: " << table.get<std::string>("indicator") << "Errno: " << al_get_errno();
		}

		m_lineHeight = al_get_font_line_height(m_theme->font());

		// Load messages
		sol::table msgTable = table.get<sol::table>("messages");
		if (!msgTable.valid() || msgTable.empty())
		{
			LOG_S(ERROR) << "Textbox table \"messages\" is invalid or empty.";
		}
		else
		{
			// Iterate over every message.
			msgTable.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_messages.push_back(pair.second.as<std::string>());
			});
		}

		// Connects receive() method automatically.
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
		if (m_isVisible)
		{
			if (e.m_keycode == sl::Keys::UI_CONFIRM)
			{
				// Make sure when going to the next "page" of text in a textbox that it doesn't trigger an out of bounds exception.
				++m_page;
				if (m_page > (m_messages.size() - 1))
				{
					m_page = (m_messages.size() - 1);
				}

				// Reset animations for the next page.
				m_characterIndex = 0;
				m_timePassed = 0.0;
			}
		}
	}

	void Textbox::update(const double dt)
	{
		if (m_isVisible && m_duration > 0)
		{
			// Check to see if its time to proceed to the next frame of animation for the text box indicator.
			m_indicatorTimePassed += dt;
			if (m_indicatorTimePassed > Time::milliseconds(500))
			{
				(m_drawLoweredIndicator == true) ? m_drawLoweredIndicator = false : m_drawLoweredIndicator = true;

				m_indicatorTimePassed = 0.0;
			}
			
			// Update time passed and check if its exceeded the duration allowed for each character.
			// If so, update the index of the character to draw in the string.
			m_timePassed += dt;
			if (m_timePassed > Time::milliseconds(m_duration))
			{
				if (!((m_characterIndex > (m_messages[m_page].size() - 1))))
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
			al_draw_bitmap(m_frame, m_bounds.m_x, m_bounds.m_y, 0);

			// Then if there is a speaker, add that.
			if (!m_speaker.empty())
			{
				al_draw_text(m_theme->font(), m_theme->colour(), m_bounds.m_x + 2, m_bounds.m_y + 2, 0, m_speaker.c_str());
			}
			
			// Then draw the relevant text.
			al_draw_multiline_text(m_theme->font(), m_theme->colour(), m_bounds.m_x + 2, m_bounds.m_y + m_lineHeight, m_maxWidth, m_lineHeight, 0, m_messages[m_page].substr(0, m_characterIndex).c_str());

			// Then draw the indicator finally.
			float x = m_bounds.m_x + m_bounds.m_width - al_get_bitmap_width(m_indicator) - 2;
			float y = m_bounds.m_y + m_bounds.m_height - al_get_bitmap_height(m_indicator) - 2;
			if (m_drawLoweredIndicator)
			{
				y += 2;
			}
			
			al_draw_bitmap(m_indicator, x, y, 0);
		}
	}
}