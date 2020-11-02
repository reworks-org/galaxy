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
		void Image::create_from_atlas(std::string_view name)
		{
			auto opt = m_theme->m_atlas.get_region(name);
			if (opt != std::nullopt)
			{
				create(opt.value(), 0);
			}
			else
			{
				PL_LOG(PL_ERROR, "Unable to create image from {0}.", name);
			}
		}

		void Image::on_event(const pr::MouseMovedEvent& mme)
		{
			if (m_region.contains(mme.m_x, mme.m_y))
			{
				m_tooltip->toggle_tooltip(true);
				m_tooltip->update_pos(mme.m_x, mme.m_y);
			}
			else
			{
				m_tooltip->toggle_tooltip(false);
			}
		}

		void Image::update()
		{
		}

		void Image::render(qs::Camera& camera)
		{
			if (m_tooltip->can_draw())
			{
				m_tooltip->render(camera);
			}
		}
	} // namespace widget
} // namespace galaxy