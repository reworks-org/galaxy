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
#include "sl/libs/sol2/sol.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Textbox.hpp"

namespace sl
{
	Textbox::Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const std::string& font, const unsigned int maxWidth, const ALLEGRO_COLOR col, const unsigned int duration, const std::string& speaker)
		:Widget({x, y, 0, 0}), m_frame(nullptr), m_indicator(nullptr), m_messages(messages), m_speaker(speaker), m_font(nullptr), m_maxWidth(maxWidth), m_colour(col), m_duration(duration), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_page(0), m_drawLoweredIndicator(false), m_lineHeight(0)
	{
		// Load frame texture.
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

		// Load indicator texture.
		m_indicator = al_load_bitmap(indicator.c_str());
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to load texture: " << indicator << "Errno: " << al_get_errno();
		}

		// Load font.
		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to retrieve font: " << font << "Errno: " << al_get_errno();
		}
		else
		{
			m_lineHeight = al_get_font_line_height(m_font);
		}

		Locator::dispatcher->sink<sl::KeyDownEvent>().connect(this);
	}

	Textbox::Textbox(const sol::table& table)
		:Widget({ 0, 0, 0, 0 }), m_frame(nullptr), m_indicator(nullptr), m_font(nullptr), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_page(0), m_drawLoweredIndicator(false), m_lineHeight(0)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get textbox data.
		m_maxWidth = table.get<unsigned int>("maxWidth");
		m_duration = table.get<unsigned int>("duration");
		m_speaker = table.get<std::string>("speaker");
		sol::table colTable = table.get<sol::table>("colour");
		if (!colTable.valid() || colTable.empty())
		{
			LOG_S(ERROR) << "Textbox table \"colour\" is invalid or empty.";
		}
		else
		{
			m_colour = al_map_rgba
			(
				table.get<unsigned char>("r"),
				table.get<unsigned char>("g"),
				table.get<unsigned char>("b"),
				table.get<unsigned char>("a")
			);
		}

		// Load frame texture.
		m_frame = al_load_bitmap(table.get<const char*>("frame"));
		if (!m_frame)
		{
			LOG_S(FATAL) << "Failed to load texture: " << table.get<const char*>("frame") << "Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_frame);
			m_bounds.m_height = al_get_bitmap_height(m_frame);
		}

		// Load indicator texture.
		m_indicator = al_load_bitmap(table.get<const char*>("indicator"));
		if (!m_indicator)
		{
			LOG_S(FATAL) << "Failed to load texture: " << table.get<const char*>("indicator") << "Errno: " << al_get_errno();
		}

		// Load font.
		m_font = Locator::fontBook->get(table.get<std::string>("font"));
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to retrieve font: " << table.get<std::string>("font") << "Errno: " << al_get_errno();
		}
		else
		{
			m_lineHeight = al_get_font_line_height(m_font);
		}

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

		// Attach event.
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
			if (e.m_keycode == sl::Keys::TEXTBOX_CONFIRM)
			{
				++m_page;
				if (m_page > (m_messages.size() - 1))
				{
					m_page = (m_messages.size() - 1);
				}

				m_characterIndex = 0;
				m_timePassed = 0.0;
			}
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
			al_draw_bitmap(m_frame, m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);

			// Then if there is a speaker, add that.
			if (!m_speaker.empty())
			{
				al_draw_text(m_font, m_colour, m_bounds.m_x + m_offsetX + 2, m_bounds.m_y + m_offsetY + 2, 0, m_speaker.c_str());
			}
			
			// Then draw the relevant text.
			al_draw_multiline_text(m_font, m_colour, m_bounds.m_x + m_offsetX + 2, m_bounds.m_y + m_offsetY + m_lineHeight, m_maxWidth, m_lineHeight, 0, m_messages[m_page].substr(0, m_characterIndex).c_str());

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