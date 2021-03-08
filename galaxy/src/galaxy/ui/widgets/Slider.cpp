///
/// Slider.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>
#include <chrono>

#include "Slider.hpp"

namespace galaxy
{
	namespace ui
	{
		Slider::Slider() noexcept
		    : m_value {1.0f}, m_pressed {false}, m_marker_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Slider::create(std::string_view slider, std::string_view marker)
		{
			m_slider.create(slider);
			m_marker.create(marker);

			m_bounds.m_width  = m_slider.get_width();
			m_bounds.m_height = m_slider.get_height();

			m_theme->m_sb.add(&m_slider, &m_slider_transform, 0);
			m_theme->m_sb.add(&m_marker, &m_marker_transform, 1);
		}

		void Slider::on_event(const events::MouseMoved& mme) noexcept
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				if (m_pressed)
				{
					m_value = std::clamp((static_cast<float>(mme.m_x) - m_bounds.m_x) / m_bounds.m_width, 0.0f, 1.0f);
				}

				if (m_tooltip)
				{
					if (m_pressed)
					{
						m_tooltip->can_draw(false);
					}
					else
					{
						m_tooltip->can_draw(true);
						m_tooltip->update_pos(mme.m_x, mme.m_y);
					}
				}
			}
		}

		void Slider::on_event(const events::MousePressed& mpe) noexcept
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_pressed = true;
				m_value   = std::clamp((mpe.m_x - m_bounds.m_x) / m_bounds.m_width, 0.0f, 1.0f);

				if (m_sound != nullptr)
				{
					m_sound->play();
				}
			}
		}

		void Slider::on_event(const events::MouseReleased& mre) noexcept
		{
			m_pressed = false;

			if (m_tooltip)
			{
				if (m_bounds.contains(mre.m_x, mre.m_y))
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mre.m_x, mre.m_y);
				}
			}
		}

		void Slider::update(const double dt)
		{
			m_marker_transform.set_pos((m_bounds.m_x + (m_bounds.m_width * m_value)) - (m_marker.get_width() / 2.0f), m_bounds.m_y - (m_marker.get_height() / 2.0f) + (m_bounds.m_height / 2.0f));
		}

		void Slider::render()
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render();
				}
			}
		}

		void Slider::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_slider_transform.set_pos(x, y);
		}

		const float Slider::value() const noexcept
		{
			return m_value;
		}

		const float Slider::percentage() const noexcept
		{
			return m_value * 100.0f;
		}
	} // namespace ui
} // namespace galaxy