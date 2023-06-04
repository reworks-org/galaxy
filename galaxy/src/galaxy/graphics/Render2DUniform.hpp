///
/// Render2DUniform.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDER2DUNIFORM_HPP_
#define GALAXY_GRAPHICS_RENDER2DUNIFORM_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26495)
#endif

namespace galaxy
{
	namespace graphics
	{
		///
		/// Uniform data passed to shader from entity.
		///
		struct Render2DUniform final
		{
			///
			/// Orthographic transform.
			///
			alignas(16) glm::mat4 transform;

			///
			/// Colour / tint.
			///
			alignas(16) glm::vec4 colour;

			///
			/// Entity ID.
			///
			int entity;

			///
			/// Is this being rendered with GL_POINTS.
			///
			bool point;

			///
			/// Does this renderable use a texture.
			///
			bool textured;

			///
			/// Does this renderable use a normal map.
			///
			bool normal_mapped;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif