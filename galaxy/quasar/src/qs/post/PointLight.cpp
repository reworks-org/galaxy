///
/// PointLight.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "PointLight.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// PostEffect identifier.
	///
	namespace effect
	{
		PointLight::PointLight()
		    : m_z_level {0.0f}, m_pos {0.0f, 0.0f}, m_falloff {0.0f, 0.0f, 1.0f}, m_light_colour {1.0f}, m_ambient_colour {1.0f}
		{
		}

		void PointLight::apply_uniforms()
		{
			m_shader.set_uniform("u_falloff", m_falloff);
			m_shader.set_uniform("u_light_pos", glm::vec3(m_pos.x, m_pos.y, m_z_level));
			m_shader.set_uniform("u_light_colour", m_light_colour);
			m_shader.set_uniform("u_ambient_colour", m_ambient_colour);
		}
	} // namespace effect
} // namespace qs