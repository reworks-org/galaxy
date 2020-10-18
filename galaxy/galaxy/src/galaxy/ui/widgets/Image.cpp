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
		Image::Image()
		{
		}

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

		void Image::update()
		{
		}

		void Image::render(qs::Camera& camera)
		{
		}
	} // namespace widget
} // namespace galaxy