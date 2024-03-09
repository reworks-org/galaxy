///
/// Textures.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_TEXTURES_HPP_
#define GALAXY_RESOURCE_TEXTURES_HPP_

#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for texture cache.
		///
		using Textures = Cache<graphics::Texture2D, Loader<graphics::Texture2D>>;
	} // namespace resource
} // namespace galaxy

#endif
