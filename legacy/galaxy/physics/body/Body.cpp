///
/// Body.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Body.hpp"

namespace galaxy
{
	namespace physics
	{
		void Body::update_aabb(const float x, const float y) noexcept
		{
			m_aabb.m_min.x = x;
			m_aabb.m_min.y = y;

			m_aabb.m_max.x = x + m_size.x;
			m_aabb.m_max.y = y + m_size.y;
		}

		const float Body::mass() const noexcept
		{
			return m_mass;
		}

		Body::Body(const float density, const glm::vec2& size) noexcept
		    : m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}, m_mass {density * (size.x * size.y)}, m_size {size}
		{
		}

		Body::Body(const float density, const float width, const float height) noexcept
		    : m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}, m_mass {density * (width * height)}, m_size {width, height}
		{
		}
	} // namespace physics
} // namespace galaxy