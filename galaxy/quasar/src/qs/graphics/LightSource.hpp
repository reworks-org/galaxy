///
/// LightSource.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LIGHTSOURCE_HPP_
#define QUASAR_LIGHTSOURCE_HPP_

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Holds data relating to a "Light Source" for use in GLSL.
	///
	struct LightSource final
	{
		///
		/// Default constructor.
		///
		LightSource() noexcept;

		///
		/// Z level of light, changes the way the light reflects off the normal map.
		///
		float m_z_level;

		///
		/// XY location of light source.
		///
		glm::vec2 m_pos;

		///
		/// Falloff range for the light source.
		/// Controls intensity.
		///
		glm::vec3 m_falloff;

		///
		/// Colour of the light.
		///
		glm::vec4 m_light_colour;

		///
		/// Colour of the ambient light.
		///
		glm::vec4 m_ambient_colour;
	};
} // namespace qs

#endif