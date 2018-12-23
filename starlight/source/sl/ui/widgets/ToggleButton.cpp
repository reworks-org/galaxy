///
/// ToggleButton.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "ToggleButton.hpp"

namespace sl
{
	ToggleButton::ToggleButton(const int x, const int y, const std::array<std::string, 3>& textures)
		:Widget({x, y, 0, 0}), m_callback(nullptr)
	{
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<ToggleButton, &ToggleButton::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ToggleButton, &ToggleButton::recieveMoved>(this);

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = al_load_bitmap(textures[i].c_str());
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to load texture: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);
	}

	ToggleButton::ToggleButton(const sol::table& table)
		:Widget({0, 0, 0, 0}), m_callback(nullptr)
	{
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<ToggleButton, &ToggleButton::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ToggleButton, &ToggleButton::recieveMoved>(this);

		// Get position data.
		m_bounds.m_x = table.get<int>("x");
		m_bounds.m_y = table.get<int>("y");

		// Get texture data.
		std::array<std::string, 3> textures =
		{
			table.get<std::string>("offTexture"),
			table.get<std::string>("onTexture"),
			table.get<std::string>("hoverTexture")
		};

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = al_load_bitmap(textures[i].c_str());
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to load texture: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);
	}

	ToggleButton::~ToggleButton()
	{
		for (ALLEGRO_BITMAP* texture : m_textures)
		{
			if (texture)
			{
				al_destroy_bitmap(texture);
			}
		}
	}

	void ToggleButton::update(const double dt)
	{
	}

	void ToggleButton::render()
	{
		if (m_isVisible)
		{
			// Simply render depending on ToggleButton state.
			switch (m_state)
			{
			case ToggleButton::State::OFF:
				al_draw_bitmap(m_textures[0], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case ToggleButton::State::ON:
				al_draw_bitmap(m_textures[1], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case ToggleButton::State::HOVER:
				al_draw_bitmap(m_textures[2], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);

				if (m_tooltip)
				{
					m_tooltip->draw();
				}
				break;
			}
		}
	}

	void ToggleButton::receivePress(const sl::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the ToggleButton, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if (((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom)) && e.m_button == 1)
			{
				if (m_state == ToggleButton::State::OFF || m_state == ToggleButton::State::HOVER)
				{
					m_state = ToggleButton::State::ON;
					m_callback();
				}
				else
				{
					ToggleButton::State::OFF;
				}
			}
		}
	}

	void ToggleButton::recieveMoved(const sl::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			// If the mouse cursor is greater than the x axis but less than the total width of the ToggleButton, and
			// Less than the height of the cursor, but greather than the y of the cursor take its height.

			int topleft = m_bounds.m_x + m_offsetX;
			int topright = topleft + m_bounds.m_width;
			int top = m_bounds.m_y + m_offsetY;
			int bottom = top + m_bounds.m_height;

			if ((e.m_x >= topleft) && (e.m_x <= topright) && (e.m_y >= top) && (e.m_y <= bottom))
			{
				if (m_state != ToggleButton::State::ON)
				{
					m_state = ToggleButton::State::HOVER;
				}
			}
			else
			{
				if (m_state != ToggleButton::State::ON)
				{
					m_state = ToggleButton::State::OFF;
				}
			}
		}
	}

	void ToggleButton::registerCallback(const std::function<void()>& callback)
	{
		m_callback = callback;
	}
}