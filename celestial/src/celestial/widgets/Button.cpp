///
/// Button.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Button.hpp"

///
/// Core namespace.
///
namespace celestial
{
	Button::Button(const int x, const int y, const std::string& label, const std::array<std::string, 3>& textures, UITheme* theme)
		:Widget({x, y, 0, 0}, theme), m_label(label), m_xLabelPos(0.0f), m_yLabelPos(0.0f), m_state(Button::State::DEFAULT), m_pressed(false)
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

	Button::Button(const int x, const int y, const std::string& text, const std::array<protostar::Colour, 3>& colours, UITheme* theme)
		:Widget({ x, y, 0, 0 }, theme), m_label(""), m_xLabelPos(0.0f), m_yLabelPos(0.0f), m_state(Button::State::DEFAULT), m_pressed(false)
	{	
		// Find correct button size.
		m_bounds.m_width = m_theme->loader()->getTextWidth(m_theme->font(), text.c_str());
		m_bounds.m_height = m_theme->loader()->getTextHeight(m_theme->font(), text.c_str());

		for (auto i = 0; i < 3; ++i)
		{
			m_textures[i] = m_theme->loader()->createRectangle(m_bounds.m_width, m_bounds.m_height, colours[i]);
		}
	}

	Button::~Button()
	{
		m_textures[0].reset();
		m_textures[1].reset();
		m_textures[2].reset();
	}

	void Button::update(const double dt)
	{
		if (m_isVisible)
		{
			if (m_state == Button::State::PRESSED)
			{
				m_pressed = true;
			}
			else
			{
				m_pressed = false;
			}
		}
	}

	void Button::render(celestial::Renderer* renderer)
	{
		if (m_isVisible)
		{
			// Simply render depending on button state.
			switch (m_state)
			{
			case Button::State::DEFAULT:
				renderer->drawTexture(m_textures[0].get(), m_bounds.m_x, m_bounds.m_y);
				renderer->drawText(m_theme->font(), m_theme->colour(), m_label, m_xLabelPos, m_yLabelPos);
				break;

			case Button::State::PRESSED:
				renderer->drawTexture(m_textures[1].get(), m_bounds.m_x, m_bounds.m_y);
				renderer->drawText(m_theme->font(), m_theme->colour(), m_label, m_xLabelPos, m_yLabelPos);
				break;

			case Button::State::HOVER:
				renderer->drawTexture(m_textures[2].get(), m_bounds.m_x, m_bounds.m_y);
				renderer->drawText(m_theme->font(), m_theme->colour(), m_label, m_xLabelPos, m_yLabelPos);

				if (m_tooltip)
				{
					m_tooltip->draw(renderer);
				}
				break;
			}
		}
	}

	void Button::receivePress(const protostar::MousePressedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y) && e.m_button == 1)
			{
				m_state = Button::State::PRESSED;
			}
		}
	}

	void Button::receiveRelease(const protostar::MouseReleasedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::recieveMoved(const protostar::MouseMovedEvent& e)
	{
		if (m_isVisible)
		{
			if (contains(e.m_x, e.m_y))
			{
				m_state = Button::State::HOVER;
			}
			else
			{
				m_state = Button::State::DEFAULT;
			}
		}
	}

	void Button::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;

		m_xLabelPos = ((m_theme->loader()->getTextureWidth(m_textures[0].get()) / 2.0f) - m_theme->loader()->getTextWidth(m_theme->font(), m_label) / 2.0f) + m_bounds.m_x;
		m_yLabelPos = ((m_theme->loader()->getTextureHeight(m_textures[0].get()) / 2.0f) - m_theme->loader()->getTextHeight(m_theme->font(), m_label) / 2.0f) + m_bounds.m_y;
	}

	const bool Button::isPressed() const noexcept
	{
		return m_pressed;
	}
}