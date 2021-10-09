///
/// Spot.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_LIGHT_SPOT_HPP_
#define GALAXY_GRAPHICS_LIGHT_SPOT_HPP_

#include <glm/vec3.hpp>

namespace galaxy
{
	namespace light
	{
		///
		/// Spot light in a shader. GLSL compatible.
		///
		struct Spot final
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
			/// Position of the light.
			///
			alignas(16) glm::vec3 m_pos;

			///
			/// Direction of the light.
			///
			alignas(16) glm::vec3 m_dir;

			///
			/// Inner cutoff angle.
			///
			alignas(4) float m_inner_cutoff;

			///
			/// Outer cutoff angle.
			///
			alignas(4) float m_outer_cutoff;
		};
	} // namespace light
} // namespace galaxy

#endif