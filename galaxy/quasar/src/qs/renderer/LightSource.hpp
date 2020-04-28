///
/// LightSource.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_LIGHTSOURCE_HPP_
#define QUASAR_LIGHTSOURCE_HPP_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "qs/core/Shader.hpp"

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
		/// Colour of the light.
		///
		glm::vec4 m_lightColour;

		///
		/// Colour of the ambient light.
		///
		glm::vec4 m_ambientColour;

		///
		/// Falloff range for the light source.
		/// Controls intensity.
		///
		glm::vec3 m_falloff;

		///
		/// XY location of light source.
		///
		glm::vec2 m_pos;
		
		///
		/// Z level of light, changes the way the light reflects off the normal map.
		///
		float m_zLevel;

		///
		/// Shader that this light source uses.
		///
		qs::Shader m_shader;
	};
}

#endif