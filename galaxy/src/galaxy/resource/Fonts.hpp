///
/// Fonts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_FONTS_HPP_
#define GALAXY_RESOURCE_FONTS_HPP_

#include "galaxy/graphics/text/Font.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for font cache.
		///
		using Fonts = Cache<graphics::Font, Loader<graphics::Font>>;
	} // namespace resource
} // namespace galaxy

#endif
