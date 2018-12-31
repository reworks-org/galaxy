///
/// UITheme.cpp
/// starlight
///
/// Created by reworks on 15/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "UITheme.hpp"

namespace sl
{
	UITheme::UITheme(const std::string& font, const ALLEGRO_COLOR colour, const std::string& masterTexture)
		:m_font(nullptr), m_colour(colour), m_masterTexture(nullptr)
	{
		// Retrieve font and check for validity.
		m_font = Locator::fontBook->get(font);
		if (!m_font)
		{
			LOG_S(FATAL) << "Could not retrieve font: " << font;
		}

		// Load and verify the master texture.
		m_masterTexture = al_load_bitmap(masterTexture.c_str());
		if (!m_masterTexture)
		{
			LOG_S(FATAL) << "Failed to load texture: " << masterTexture << " Errno: " << al_get_errno();
		}
	}

	UITheme::UITheme(const sol::table& table)
		:m_font(nullptr), m_masterTexture(nullptr)
	{
		// Retrieve font and check for validity.
		m_font = Locator::fontBook->get(table.get<std::string>("font"));
		if (!m_font)
		{
			LOG_S(FATAL) << "Could not retrieve font: " << table.get<std::string>("font");
		}

		// Load and verify the master texture.
		m_masterTexture = al_load_bitmap(table.get<std::string>("masterTexture").c_str());
		if (!m_masterTexture)
		{
			LOG_S(FATAL) << "Failed to load texture: " << table.get<std::string>("masterTexture") << " Errno: " << al_get_errno();
		}

		// Colour table.
		sol::table colTable = table.get<sol::table>("colour");
		if (!colTable.valid() || colTable.empty())
		{
			LOG_S(ERROR) << "UITheme table \"colour\" is invalid or empty.";
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
	}

	UITheme::~UITheme()
	{
		// Cleanup, etc...
		if (m_masterTexture)
		{
			al_destroy_bitmap(m_masterTexture);
		}

		m_widgetRegions.clear();
	}

	void UITheme::defineWidgetTexture(const std::string& id, const sl::Rect<int>& dim)
	{
		m_widgetRegions.emplace(id, dim);
	}

	ALLEGRO_BITMAP* UITheme::widgetTexture(const std::string& id)
	{
		sl::Rect<int> region = m_widgetRegions[id];
		return al_create_sub_bitmap(m_masterTexture, region.m_x, region.m_y, region.m_width, region.m_height);
	}

	ALLEGRO_FONT* UITheme::font() const
	{
		return m_font;
	}

	ALLEGRO_COLOR UITheme::colour() const
	{
		return m_colour;
	}
}