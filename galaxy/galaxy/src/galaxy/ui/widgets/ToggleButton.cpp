///
/// ToggleButton.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ToggleButton.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	ToggleButton::ToggleButton(const int x, const int y, const std::array<std::string, 3>& textures, UITheme* theme)
		:Widget({x, y, 0, 0}, theme), m_state(ToggleButton::State::OFF)
	{
		// Load each bitmap from the array and check for errors.
		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = m_theme->extractWidgetTexture(textures[i]);
		}

		// Set dimensions.
		m_bounds.m_width = m_theme->loader()->getTextureWidth(m_textures[0].get());
		m_bounds.m_height = m_theme->loader()->getTextureHeight(m_textures[0].get());
	}

	ToggleButton::~ToggleButton()
	{
		m_textures[0].reset();
		m_textures[1].reset();
		m_textures[2].reset();
	}

	void ToggleButton::update(const double dt)
	{
	}

	void ToggleButton::render(galaxy::Renderer* renderer)
	{
		if (m_isVisible)
		{
			// Simply render depending on ToggleButton state.
			switch (m_state)
			{
			case ToggleButton::State::OFF:
				renderer->drawTexture(m_textures[0].get(), m_bounds.m_x, m_bounds.m_y);
				break;

			case ToggleButton::State::ON:
				renderer->drawTexture(m_textures[1].get(), m_bounds.m_x, m_bounds.m_y);
				break;

			case ToggleButton::State::HOVER:
				renderer->drawTexture(m_textures[2].get(), m_bounds.m_x, m_bounds.m_y);

				if (m_tooltip)
				{
					m_tooltip->draw(renderer);
				}
				break;
			}
		}
	}

	void ToggleButton::receivePress(const pr::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && e.m_button == 1)
			{
				if (m_state == ToggleButton::State::OFF || m_state == ToggleButton::State::HOVER)
				{
					m_state = ToggleButton::State::ON;
					//
				}
				else
				{
					ToggleButton::State::OFF;
				}
			}
		}
	}

	void ToggleButton::recieveMoved(const pr::MouseMovedEvent& e)
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

	void ToggleButton::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}