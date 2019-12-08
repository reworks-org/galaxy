///
/// ToggleButton.cpp
/// galaxy
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/libs/entt/signal/dispatcher.hpp"

#include "ToggleButton.hpp"

namespace galaxy
{
	ToggleButton::ToggleButton(const int x, const int y, const std::array<std::string, 3>& textures, UITheme* theme)
		:Widget({x, y, 0, 0}, theme), m_callback(nullptr)
	{
		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = m_theme->widgetTexture(textures[i]);
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to create sub bitmap: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);

		// Register events.
		galaxy::Locator::dispatcher->sink<galaxy::MousePressedEvent>().connect<ToggleButton, &ToggleButton::receivePress>(this);
		galaxy::Locator::dispatcher->sink<galaxy::MouseMovedEvent>().connect<ToggleButton, &ToggleButton::recieveMoved>(this);
	}

	ToggleButton::ToggleButton(const sol::table& table, UITheme* theme)
		:Widget({0, 0, 0, 0}, theme), m_callback(nullptr)
	{
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
			m_textures[i] = m_theme->widgetTexture(textures[i]);
			if (!m_textures[i])
			{
				LOG_S(FATAL) << "Failed to create sub bitmap: " << textures[i] << " Errno: " << al_get_errno();
			}
		}

		// Set dimensions.
		m_bounds.m_width = al_get_bitmap_width(m_textures[0]);
		m_bounds.m_height = al_get_bitmap_height(m_textures[0]);

		// Register events.
		galaxy::Locator::dispatcher->sink<galaxy::MousePressedEvent>().connect<ToggleButton, &ToggleButton::receivePress>(this);
		galaxy::Locator::dispatcher->sink<galaxy::MouseMovedEvent>().connect<ToggleButton, &ToggleButton::recieveMoved>(this);
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
				al_draw_bitmap(m_textures[0], m_bounds.m_x, m_bounds.m_y, 0);
				break;

			case ToggleButton::State::ON:
				al_draw_bitmap(m_textures[1], m_bounds.m_x, m_bounds.m_y, 0);
				break;

			case ToggleButton::State::HOVER:
				al_draw_bitmap(m_textures[2], m_bounds.m_x, m_bounds.m_y, 0);

				if (m_tooltip)
				{
					m_tooltip->draw();
				}
				break;
			}
		}
	}

	void ToggleButton::receivePress(const galaxy::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && e.m_button == 1)
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

	void ToggleButton::recieveMoved(const galaxy::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
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

	void ToggleButton::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}