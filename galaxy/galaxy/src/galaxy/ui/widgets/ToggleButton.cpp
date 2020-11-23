///
/// ToggleButton.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "ToggleButton.hpp"

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
		ToggleButton::ToggleButton()
		    : m_state {ToggleButton::State::OFF}, m_on {false}
		{
		}

		void ToggleButton::create(std::string_view on, std::string_view off, std::string_view on_hover, std::string_view off_hover)
		{
			auto opt0 = m_theme->m_atlas.get_region(on);
			if (opt0 != std::nullopt)
			{
				create(opt0.value(), 0);
				m_regions[0] = opt0.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", on);
			}

			auto opt1 = m_theme->m_atlas.get_region(off);
			if (opt1 != std::nullopt)
			{
				m_regions[1] = opt1.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", off);
			}

			auto opt2 = m_theme->m_atlas.get_region(on_hover);
			if (opt2 != std::nullopt)
			{
				m_regions[2] = opt2.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", on_hover);
			}

			auto opt3 = m_theme->m_atlas.get_region(off_hover);
			if (opt3 != std::nullopt)
			{
				m_regions[3] = opt3.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", off_hover);
			}

			m_bounds.m_width  = m_regions.at(0).m_width;
			m_bounds.m_height = m_regions.at(0).m_height;
		}

		void ToggleButton::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f});
			m_dirty       = true;
		}

		void ToggleButton::on_event(const pr::MouseMovedEvent& mme)
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				if (m_on)
				{
					m_state = ToggleButton::State::ON;
				}
				else
				{
					m_state = ToggleButton::State::OFF;
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::on_event(const pr::MousePressedEvent& mpe)
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_on = !m_on;
				if (m_on)
				{
					m_state = ToggleButton::State::ON_HOVER;
				}
				else
				{
					m_state = ToggleButton::State::OFF_HOVER;
				}

				if (m_callback != nullptr)
				{
					m_callback(m_on);
				}

				if (m_sound != nullptr)
				{
					m_sound->play();
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::update(const double dt)
		{
			switch (m_state)
			{
				case ToggleButton::State::ON:
					update_region(m_regions[0]);
					break;

				case ToggleButton::State::OFF:
					update_region(m_regions[1]);
					break;

				case ToggleButton::State::ON_HOVER:
					update_region(m_regions[2]);
					break;

				case ToggleButton::State::OFF_HOVER:
					update_region(m_regions[3]);
					break;
			}
		}

		void ToggleButton::render(qs::Camera& camera)
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}

		const bool ToggleButton::is_on() const
		{
			return m_on;
		}
	} // namespace widget
} // namespace galaxy