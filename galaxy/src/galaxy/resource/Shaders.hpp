///
/// Shaders.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SHADERS_HPP_
#define GALAXY_RESOURCE_SHADERS_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Abbreviation for font cache.
		///
		using Shaders = Cache<graphics::Shader, Loader<graphics::Shader>>;
	} // namespace resource
} // namespace galaxy

#endif
