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
		    : m_value {1.0f}, m_pressed {false}, m_marker_pos {0.0f, 0.0f, 0.0f, 0.0f}
		{
		}

		void Slider::create(std::string_view slider, std::string_view marker)
		{
			auto slider_opt = m_theme->m_atlas.get_region(slider);
			auto marker_opt = m_theme->m_atlas.get_region(marker);

			if (slider_opt != std::nullopt)
			{
				m_slider.create(slider_opt.value(), 0);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create slider widget from {0}.", slider);
			}

			m_bounds.m_width  = m_slider.get_width();
			m_bounds.m_height = m_slider.get_height();

			if (marker_opt != std::nullopt)
			{
				m_marker.create(marker_opt.value(), 1);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create slider marker from {0}.", marker);
			}
		}

		void Slider::on_event(const pr::MouseMovedEvent& mme)
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

		void Slider::on_event(const pr::MousePressedEvent& mpe)
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

		void Slider::on_event(const pr::MouseReleasedEvent& mre)
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
			m_marker.set_pos((m_bounds.m_x + (m_bounds.m_width * m_value)) - (m_marker.get_width() / 2.0f), m_bounds.m_y - (m_marker.get_height() / 2.0f) + (m_bounds.m_height / 2.0f));
		}

		void Slider::render(qs::Camera& camera)
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		void Slider::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_slider.set_pos(x, y);
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