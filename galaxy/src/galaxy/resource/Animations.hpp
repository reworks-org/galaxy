///
/// Animations.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_ANIMATIONS_HPP_
#define GALAXY_RESOURCE_ANIMATIONS_HPP_

#include "galaxy/graphics/anim/Animation.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for animation cache.
		///
		using Animations = Cache<graphics::Animation, Loader<graphics::Animation>>;
	} // namespace resource
} // namespace galaxy

#endif
