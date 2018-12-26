///
/// TextInput.cpp
/// starlight
///
/// Created by reworks on 24/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "TextInput.hpp"

namespace sl
{
	TextInput::TextInput(const sl::Rect<int>& bounds, const ALLEGRO_COLOR field, const ALLEGRO_COLOR indicator, const std::string& font, const ALLEGRO_COLOR textCol)
		:Widget(bounds), m_text(""), m_isSelected(false), m_field(nullptr), m_indicator(nullptr), m_font(nullptr), m_colour(textCol)
	{
		// Load font.
		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to load font: " << font << " Errno: " << al_get_errno();
		}

		// Create textures
		m_background = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar background bitmap.  Errno: " << al_get_errno();
		}

		m_bar = al_create_bitmap(m_bounds.m_width, m_bounds.m_height);
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}

		// Create bar bitmap.
		al_set_target_bitmap(m_bar);
		al_clear_to_color(foreCol);
		al_flip_display();

		// Create background bitmap.
		al_set_target_bitmap(m_background);
		al_clear_to_color(backCol);
		al_flip_display();

		// Restore window.
		al_set_target_backbuffer(Locator::window->getDisplay());
	}

	TextInput::TextInput(const int x, const int y, const std::string& field, const std::string& indicator, const std::string& font, const ALLEGRO_COLOR textCol)
		:Widget({ x, y, 0, 0 }), m_text(""), m_isSelected(false), m_field(nullptr), m_indicator(nullptr), m_font(nullptr), m_colour(textCol)
	{
		// Load font.
		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to load font: " << font << " Errno: " << al_get_errno();
		}

		// Create textures
		m_background = al_load_bitmap(field.c_str());
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar background bitmap.  Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_background);
			m_bounds.m_height = al_get_bitmap_height(m_background);
		}

		m_bar = al_load_bitmap(indicator.c_str());
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}
	}

	TextInput::TextInput(const sol::table& table)
		:Widget({0, 0, 0, 0}), m_text(""), m_isSelected(false), m_field(nullptr), m_indicator(nullptr), m_font(nullptr)
	{
		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Load font.
		m_font = Locator::fontBook->get(table.get<std::string>("font"));
		if (!m_font)
		{
			LOG_S(FATAL) << "Failed to load font: " << font << " Errno: " << al_get_errno();
		}

		// Create textures
		m_background = al_load_bitmap(table.get<const char*>("field"));
		if (!m_background)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar background bitmap.  Errno: " << al_get_errno();
		}
		else
		{
			// Set dimensions.
			m_bounds.m_width = al_get_bitmap_width(m_background);
			m_bounds.m_height = al_get_bitmap_height(m_background);
		}

		m_bar = al_load_bitmap(table.get<const char*>("indicator"));
		if (!m_bar)
		{
			LOG_S(FATAL) << "Failed to create ProgressBar bar bitmap. Errno: " << al_get_errno();
		}

		sol::table colTable = table.get<sol::table>("colTable");
		if (!colTable.valid() || colTable.empty())
		{
			LOG_S(ERROR) << "TextInput table \"colTable\" is invalid or empty.";
		}
		else
		{
			m_colour = al_map_rgba
			(
				colTable.get<unsigned char>("r"),
				colTable.get<unsigned char>("g"),
				colTable.get<unsigned char>("b"),
				colTable.get<unsigned char>("a")
			);
		}
	}
	
	TextInput::~TextInput() noexcept
	{
		if (m_field)
		{
			al_destroy_bitmap(m_field);
		}

		if (m_indicator)
		{
			al_destroy_bitmap(m_indicator);
		}
	}

	void TextInput::receiveMousePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the button, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if (((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom)) && e.m_button == 1)
			{
				m_isSelected = true;
			}
		}
	}

	void TextInput::receiveKeyPress(const sl::KeyDownEvent& e)
	{
		if (m_isVisible && m_isSelected)
		{

		}
	}

	void TextInput::update(const double dt)
	{
	}

	void TextInput::render()
	{
		// 
	}
}