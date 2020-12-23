///
/// Button.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <qs/core/Renderer.hpp>

#include "Button.hpp"

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
		Button::Button()
		    : m_state {Button::State::DEFAULT}
		{
		}

		void Button::create(std::string_view def, std::string_view pressed, std::string_view hover)
		{
			auto opt0 = m_theme->m_atlas.get_region(def);
			if (opt0 != std::nullopt)
			{
				create(opt0.value(), 0);
				m_regions[0] = opt0.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", def);
			}

			auto opt1 = m_theme->m_atlas.get_region(pressed);
			if (opt1 != std::nullopt)
			{
				m_regions[1] = opt1.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", pressed);
			}

			auto opt2 = m_theme->m_atlas.get_region(hover);
			if (opt2 != std::nullopt)
			{
				m_regions[2] = opt2.value();
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create button sprite from {0}.", hover);
			}

			m_bounds.m_width  = m_regions.at(0).m_width;
			m_bounds.m_height = m_regions.at(0).m_height;
		}

		void Button::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_pos.x      = x;
			m_pos.y      = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f});
			m_dirty       = true;
		}

		void Button::on_event(const pr::MouseMovedEvent& mme)
		{
			if (m_bounds.contains(mme.m_x, mme.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::on_event(const pr::MousePressedEvent& mpe)
		{
			if (m_bounds.contains(mpe.m_x, mpe.m_y))
			{
				m_state = Button::State::PRESSED;
				if (m_callback != nullptr)
				{
					m_callback();
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

		void Button::on_event(const pr::MouseReleasedEvent& mre)
		{
			if (m_bounds.contains(mre.m_x, mre.m_y))
			{
				m_state = Button::State::HOVER;

				if (m_tooltip)
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mre.m_x, mre.m_y);
				}
			}
			else
			{
				m_state = Button::State::DEFAULT;

				if (m_tooltip)
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Button::update(const double dt)
		{
			switch (m_state)
			{
				case Button::State::DEFAULT:
					update_region(m_regions[0]);
					break;

				case Button::State::PRESSED:
					update_region(m_regions[1]);
					break;

				case Button::State::HOVER:
					update_region(m_regions[2]);
					break;
			}
		}

		void Button::render(qs::Camera& camera)
		{
			if (m_tooltip)
			{
				if (m_tooltip->can_draw())
				{
					m_tooltip->render(camera);
				}
			}
		}
	} // namespace widget
} // namespace galaxy