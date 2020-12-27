///
/// KineticBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "KineticBody.hpp"

namespace galaxy
{
	namespace physics
	{
		KineticBody::KineticBody(const float density, const glm::vec2& size)
		    : Body {density, size}, m_force {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}
		{
		}

		KineticBody::KineticBody(const float density, const float width, const float height)
		    : Body {density, width, height}, m_force {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}
		{
		}

		void KineticBody::apply_force(const float x_force, const float y_force)
		{
			m_force.x = x_force;
			m_force.y = y_force;
		}

		void KineticBody::apply_force(glm::vec2& force)
		{
			m_force = std::move(force);
		}

		void KineticBody::apply_horizontal_force(const float x_force)
		{
			m_force.x = x_force;
		}

		void KineticBody::apply_vertical_force(const float y_force)
		{
			m_force.y = y_force;
		}

		const glm::vec2& KineticBody::get_force() const
		{
			return m_force;
		}

		const glm::vec2& KineticBody::get_vel() const
		{
			return m_velocity;
		}

		const bool KineticBody::is_rigid() const
		{
			return false;
		}
	} // namespace physics
} // namespace galaxy