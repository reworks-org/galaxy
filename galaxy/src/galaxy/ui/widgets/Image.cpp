///
/// Image.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

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
		void Image::create(std::string_view name)
		{
			auto opt = m_theme->m_atlas.get_region(name);
			if (opt != std::nullopt)
			{
				create(opt.value(), 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to create image widget from {0}.", name);
			}

			m_bounds.m_width  = get_width();
			m_bounds.m_height = get_height();
		}

		void Image::set_pos(const float x, const float y)
		{
			m_bounds.m_x = x;
			m_bounds.m_y = y;
			m_pos.x      = x;
			m_pos.y      = y;

			m_translation = glm::translate(glm::mat4 {1.0f}, {x, y, 0.0f});
			m_dirty       = true;
		}

		void Image::on_event(const pr::MouseMovedEvent& mme)
		{
			if (m_tooltip)
			{
				if (m_bounds.contains(mme.m_x, mme.m_y))
				{
					m_tooltip->can_draw(true);
					m_tooltip->update_pos(mme.m_x, mme.m_y);
				}
				else
				{
					m_tooltip->can_draw(false);
				}
			}
		}

		void Image::update(const double dt)
		{
		}

		void Image::render(qs::Camera& camera)
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