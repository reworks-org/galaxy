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
	struct LightSource
	{
		glm::vec4 m_lightColour;
		glm::vec4 m_ambientColour;
		glm::vec3 m_falloff;
		float m_zLevel;
		glm::vec2 m_pos;
		qs::Shader m_shader;
	};
}

#endif