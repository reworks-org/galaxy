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

		void ToggleButton::create_from_atlas(const std::array<std::string, 4>& sprites)
		{
			auto opt = m_theme->m_atlas.get_region(sprites[0]);
			if (opt != std::nullopt)
			{
				create(opt.value(), 0);
				m_regions[0] = opt.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", sprites[0]);
			}

			auto opt2 = m_theme->m_atlas.get_region(sprites[1]);
			if (opt2 != std::nullopt)
			{
				m_regions[1] = opt2.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", sprites[1]);
			}

			auto opt3 = m_theme->m_atlas.get_region(sprites[2]);
			if (opt3 != std::nullopt)
			{
				m_regions[2] = opt3.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", sprites[2]);
			}

			auto opt4 = m_theme->m_atlas.get_region(sprites[3]);
			if (opt4 != std::nullopt)
			{
				m_regions[3] = opt4.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", sprites[3]);
			}

			m_bounds.m_width  = m_regions.at(0).m_width;
			m_bounds.m_height = m_regions.at(0).m_height;
		}

		void ToggleButton::set_pos(const float x, const float y) noexcept
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
				m_state = ToggleButton::State::ON;
				m_on    = !m_on;
				if (m_callback != nullptr)
				{
					m_callback(m_on);
				}

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void ToggleButton::on_event(const pr::MouseReleasedEvent& mre)
		{
			if (m_bounds.contains(mre.m_x, mre.m_y))
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
					m_tooltip->update_pos(mre.m_x, mre.m_y);
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

		void ToggleButton::update()
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

		const bool ToggleButton::is_on() const noexcept
		{
			return m_on;
		}
	} // namespace widget
} // namespace galaxy