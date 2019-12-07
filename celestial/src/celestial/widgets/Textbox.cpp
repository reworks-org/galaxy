///
/// Textbox.cpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#include "Textbox.hpp"

namespace celestial
{
	Textbox::Textbox(const int x, const int y, const std::string& frame, const std::string& indicator, const std::vector<std::string>& messages, const unsigned int maxWidth, const unsigned int duration, UITheme* theme, const std::string& speaker)
		:Widget({x, y, 0, 0}, theme), m_frame(nullptr), m_indicator(nullptr), m_messages(messages), m_speaker(speaker), m_maxWidth(maxWidth), m_duration(duration), m_timePassed(0.0), m_indicatorTimePassed(0.0), m_characterIndex(0), m_page(0), m_drawLoweredIndicator(false), m_lineHeight(0)
	{
		// Load frame texture.
		m_frame = m_theme->extractWidgetTexture(frame);
		
		// Set dimensions.
		m_bounds.m_w = m_theme->loader()->getTextureWidth(m_frame.get());
		m_bounds.m_h = m_theme->loader()->getTextureWidth(m_frame.get());

		// Load indicator texture.
		m_indicator = m_theme->extractWidgetTexture(indicator);
		m_lineHeight = m_theme->loader()->getLineHeight(m_theme->font());

		m_indicatorW = m_theme->loader()->getTextureWidth(m_indicator.get());
		m_indicatorH = m_theme->loader()->getTextureHeight(m_indicator.get());
	}

	Textbox::~Textbox()
	{
		m_frame.reset();
		m_indicator.reset();
		
		m_messages.clear();
	}

	void Textbox::receive(const celestial::KeyDownEvent& e)
	{
		if (m_isVisible)
		{
			if (e.m_keycode == CONFIRMATION_KEYCODE)
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
			if (m_indicatorTimePassed > TIME_COMPARE_FUNC(500))
			{
				(m_drawLoweredIndicator == true) ? m_drawLoweredIndicator = false : m_drawLoweredIndicator = true;

				m_indicatorTimePassed = 0.0;
			}
			
			// Update time passed and check if its exceeded the duration allowed for each character.
			// If so, update the index of the character to draw in the string.
			m_timePassed += dt;
			if (m_timePassed > TIME_COMPARE_FUNC(m_duration))
			{
				if (!((m_characterIndex > (m_messages[m_page].size() - 1))))
				{
					++m_characterIndex;
				}

				m_timePassed = 0.0;
			}
		}
	}

	void Textbox::render(celestial::compat::Renderer* renderer)
	{
		if (m_isVisible)
		{
			// Draw frame on the bottom.
			renderer->drawTexture(m_frame.get(), m_bounds.m_x, m_bounds.m_y);

			// Then if there is a speaker, add that.
			if (!m_speaker.empty())
			{
				renderer->drawText(m_theme->font(), m_theme->colour(), m_speaker, m_bounds.m_x + 2, m_bounds.m_y + 2);
			}
			
			// Then draw the relevant text.
			renderer->drawMultilineText(m_theme->font(), m_theme->colour(), m_messages[m_page].substr(0, m_characterIndex), m_bounds.m_x + 2, m_bounds.m_y + m_lineHeight, m_maxWidth, m_lineHeight);

			// Then draw the indicator finally.
			float x = m_bounds.m_x + m_bounds.m_w - m_indicatorW - 2;
			float y = m_bounds.m_y + m_bounds.m_h - m_indicatorH - 2;
			if (m_drawLoweredIndicator)
			{
				y += 2;
			}
			
			renderer->drawTexture(m_indicator.get(), x, y);
		}
	}

	void Textbox::setOffset(const int x, const int y)
	{
		m_bounds.m_x += x;
		m_bounds.m_y += y;
	}
}