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
#include <glm/vec4.hpp>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Light data to upload to gpu.
		///
		struct Light final
		{
			///
			/// Colour.
			///
			glm::vec4 colour;

			///
			/// Falloff range.
			///
			glm::vec3 falloff;

			///
			/// Position.
			///
			glm::vec2 pos;

			///
			/// Depth of light.
			///
			float depth = 0.075f;

			///
			/// Size of LightSource area.
			///
			float diameter = 256.0f;
		};
	} // namespace graphics
} // namespace galaxy

#endif