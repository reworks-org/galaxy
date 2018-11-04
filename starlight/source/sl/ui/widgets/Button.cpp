///
/// Button.cpp
/// starlight
///
/// Created by reworks on 19/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/ServiceLocator.hpp"
#include "sl/libs/entt/signal/dispatcher.hpp"

#include "Button.hpp"

namespace sl
{
	Button::Button(const sl::Rect<int>& bounds)
		:Widget(bounds)
	{
		Locator::dispatcher->sink<ALLEGRO_MOUSE_EVENT>().connect(this);
	}

	void Button::render()
	{
		switch (m_state)
		{
		case Button::State::DEFAULT:
			break;

		case Button::State::PRESSED:
			break;

		case Button::State::HOVER:
			break;
		}
	}

	void Button::receive(const ALLEGRO_MOUSE_EVENT& e)
	{
		// If the mouse cursor is greater than the x axis but less than the total width of the button, and
		// Less than the height of the cursor, but greather than the y of the cursor take its height.
		if (((e.x >= m_bounds.m_x) && e.x <= (m_bounds.m_x + m_bounds.m_width)) && ((e.y <= m_bounds.m_y) && (e.y >= (m_bounds.m_y - m_bounds.m_height))))
		{
			m_state = Button::State::HOVER;

			if (e.button == 1)
			{
				m_state = Button::State::PRESSED;
				// call function?
			}
		}
		else
		{
			m_state = Button::State::DEFAULT;
		}
	}
}