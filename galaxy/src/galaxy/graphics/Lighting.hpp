///
/// Lighting.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHTING_HPP_
#define GALAXY_GRAPHICS_LIGHTING_HPP_

#include <vector>

#include "galaxy/graphics/Light.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data needed to light a scene.
		///
		struct Lighting final
		{
			///
			/// Ambient lighting colour.
			///
			glm::vec4 ambient_light_colour;

			///
			/// Viewport x/y.
			///
			glm::vec2 resolution;

			///
			/// Array of lights in the scene.
			///
			std::vector<Light> lights;
		};
	} // namespace graphics
} // namespace galaxy

#endif