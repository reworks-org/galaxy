///
/// Light.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_HPP_
#define GALAXY_GRAPHICS_LIGHT_HPP_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		///
		///
		struct Light final
		{
			///
			///
			///
			glm::vec4 colour;

			///
			///
			///
			glm::vec3 falloff;

			///
			///
			///
			glm::vec2 pos;

			///
			///
			///
			float depth;
		};
	} // namespace graphics
} // namespace galaxy

#endif