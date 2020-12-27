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
		void Body::set_pos(const float x, const float y)
		{
			m_pos.x = x;
			m_pos.y = y;

			update_aabb();
		}

		void Body::update_aabb()
		{
			m_aabb.m_min.x = m_pos.x;
			m_aabb.m_min.y = m_pos.y;

			m_aabb.m_max.x = m_pos.x + m_size.x;
			m_aabb.m_max.y = m_pos.y + m_size.y;
		}

		const glm::vec2& Body::get_pos() const
		{
			return m_pos;
		}

		const float Body::mass() const
		{
			return m_mass;
		}

		Body::Body(const float density, const glm::vec2& size)
		    : m_pos {0.0f, 0.0f}, m_mass {1.0f}, m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}
		{
			m_mass = density * (size.x * size.y);
			m_size = size;

			update_aabb();
		}

		Body::Body(const float density, const float width, const float height)
		    : m_pos {0.0f, 0.0f}, m_mass {1.0f}, m_restitution {1.0f}, m_static_friction {1.0f}, m_dynamic_friction {0.5f}
		{
			m_mass   = density * (width * height);
			m_size.x = width;
			m_size.y = height;

			update_aabb();
		}
	} // namespace physics
} // namespace galaxy