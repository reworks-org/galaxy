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

		void Button::create_from_atlas(const std::array<std::string, 3>& sprites)
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

			m_bounds.m_width  = m_regions.at(0).m_width;
			m_bounds.m_height = m_regions.at(0).m_height;
		}

		void Button::set_pos(const float x, const float y) noexcept
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f}); // wants mat4 here for some reason?
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
				m_callback();

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

		void Button::update()
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