///
/// Directional.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_DIRECTIONAL_HPP_
#define GALAXY_GRAPHICS_LIGHT_DIRECTIONAL_HPP_

#include <glm/vec3.hpp>

namespace galaxy
{
	namespace light
	{
		///
		/// Directional light in a shader.  GLSL compatible.
		///
		struct Directional final
		{
			///
			/// Intensity of ambient light.
			///
			alignas(16) glm::vec3 m_ambient_intensity;

			///
			/// Intensity of diffuse light.
			///
			alignas(16) glm::vec3 m_diffuse_intensity;

			///
			/// Intensity of specular light.
			///
			alignas(16) glm::vec3 m_specular_intensity;

			///
			/// Direction of the light.
			///
			alignas(16) glm::vec3 m_dir;
		};
	} // namespace light
} // namespace galaxy

#endif