///
/// Body.cpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#include "Body.hpp"

///
/// Core namespace.
///
namespace rs
{
	Body::Body(const float density, const glm::vec2& size)
	    : m_pos {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}, m_mass {1.0f}, m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}
	{
		m_mass = density * (size.x * size.y);
	}

	Body::Body(const float density, const float width, const float height)
	    : m_pos {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}, m_mass {1.0f}, m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}
	{
		m_mass = density * (width * height);
	}
} // namespace rs