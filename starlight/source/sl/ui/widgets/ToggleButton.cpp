///
/// ToggleButton.cpp
/// starlight
///
/// Created by reworks on 16/12/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "ToggleButton.hpp"

namespace sl
{
	ToggleButton::ToggleButton(const sl::Rect<int>& bounds, const std::array<std::string, 3>& images)
		:Widget(bounds), m_callback(nullptr)
	{
		// Register events.
		sl::Locator::dispatcher->sink<sl::MousePressedEvent>().connect<ToggleButton, &ToggleButton::receivePress>(this);
		sl::Locator::dispatcher->sink<sl::MouseMovedEvent>().connect<ToggleButton, &ToggleButton::recieveMoved>(this);

		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_images[i] = al_load_bitmap(images[i].c_str());
			if (!m_images[i])
			{
				LOG_S(FATAL) << "Failed to load texture: " << images[i] << " Errno: " << al_get_errno();
			}
		}
	}

	ToggleButton::~ToggleButton()
	{
		for (ALLEGRO_BITMAP* image : m_images)
		{
			if (image)
			{
				al_destroy_bitmap(image);
			}
		}
	}

	void ToggleButton::update()
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
				al_draw_bitmap(m_images[0], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case ToggleButton::State::ON:
				al_draw_bitmap(m_images[1], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);
				break;

			case ToggleButton::State::HOVER:
				al_draw_bitmap(m_images[2], m_bounds.m_x + m_offsetX, m_bounds.m_y + m_offsetY, 0);

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