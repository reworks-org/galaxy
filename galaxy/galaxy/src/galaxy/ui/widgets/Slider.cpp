///
/// Slider.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Slider.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Widget specific namespace.
	///
	namespace widget
	{
		Slider::Slider()
		{
		}

		void Slider::create(const int x, const int y, std::string_view slider, std::string_view marker)
		{
			// Load slider texture and check for errors.
			m_slider = m_theme->extractWidgetTexture(slider);

			// Set dimensions.
			m_bounds.m_width  = m_theme->loader()->getTextureWidth(m_slider.get());
			m_bounds.m_height = m_theme->loader()->getTextureHeight(m_slider.get());

			// Load marker texture and check for errors.
			m_marker  = m_theme->extractWidgetTexture(marker);
			m_markerW = m_theme->loader()->getTextureWidth(m_marker.get());
			m_markerH = m_theme->loader()->getTextureHeight(m_marker.get());
		}

		void Slider::on_event(const pr::MouseMovedEvent& mee)
		{
			if (m_isVisible)
			{
				if (contains(e.m_x, e.m_y))
				{
					m_drawTooltip = true;

					if (e.m_pressure == 1.0f)
					{
						m_drawTooltip         = false;
						int cursorPosOnSlider = e.m_x - m_bounds.m_x;
						m_value               = std::clamp(static_cast<float>(cursorPosOnSlider) / static_cast<float>(m_bounds.m_width), 0.0f, 1.0f);
					}
				}
				else
				{
					m_drawTooltip = false;
				}
			}
		}

		void Slider::on_event(const pr::MousePressedEvent& mpe)
		{
			if (m_isVisible)
			{
				if (contains(e.m_x, e.m_y) && (e.m_button == 1))
				{
					// We take away the button top left pos, because then we are left over with the position of the cursor on the widget.
					// Then make sure the resulting value is within the bar width.
					m_drawTooltip         = false;
					int cursorPosOnSlider = e.m_x - m_bounds.m_x;
					m_value               = std::clamp(static_cast<float>(cursorPosOnSlider) / static_cast<float>(m_bounds.m_width), 0.0f, 1.0f);
				}
			}
		}

		void Slider::update()
		{
			if (m_isVisible)
			{
				// Draw marker centered on value by taking away half the width of the marker.
				m_markerX = (m_bounds.m_x + (m_bounds.m_width * m_value)) - (m_markerW / 2.0f);
			}
		}

		void Slider::render(qs::Camera& camera)
		{
			if (m_isVisible)
			{
				renderer->drawTexture(m_slider.get(), m_bounds.m_x, m_bounds.m_y);

				// we center the marker so it is exactly half way on the texture.
				renderer->drawTexture(m_marker.get(), m_markerX,                                                                             // x
						      static_cast<float>(m_bounds.m_y) - (m_markerH / 2.0f) + (static_cast<float>(m_bounds.m_height) / 2.0f) // y
				);

				if (m_tooltip && m_drawTooltip)
				{
					m_tooltip->draw(renderer);
				}
			}
		}

		const float Slider::value() const
		{
			return m_value;
		}

		const float Slider::percentage() const
		{
			return m_value * 100.0f;
		}
	} // namespace widget
} // namespace galaxy