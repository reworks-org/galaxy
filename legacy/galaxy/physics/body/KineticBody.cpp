///
/// KineticBody.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "KineticBody.hpp"

namespace galaxy
{
	namespace physics
	{
		KineticBody::KineticBody(const float density, const glm::vec2& size) noexcept
		    : Body {density, size}, m_force {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}
		{
		}

		KineticBody::KineticBody(const float density, const float width, const float height) noexcept
		    : Body {density, width, height}, m_force {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}
		{
		}

		KineticBody::KineticBody(const nlohmann::json& json)
		    : Body {json.at("density"), json.at("width"), json.at("height")}, m_force {0.0f, 0.0f}, m_velocity {0.0f, 0.0f}
		{
		}

		void KineticBody::apply_force(const float x_force, const float y_force) noexcept
		{
			m_force.x = x_force;
			m_force.y = y_force;
		}

		void KineticBody::apply_force(glm::vec2& force) noexcept
		{
			m_force = std::move(force);
		}

		void KineticBody::apply_horizontal_force(const float x_force) noexcept
		{
			m_force.x = x_force;
		}

		void KineticBody::apply_vertical_force(const float y_force) noexcept
		{
			m_force.y = y_force;
		}

		const glm::vec2& KineticBody::get_force() const noexcept
		{
			return m_force;
		}

		const glm::vec2& KineticBody::get_vel() const noexcept
		{
			return m_velocity;
		}

		const bool KineticBody::is_rigid() const noexcept
		{
			return false;
		}
	} // namespace physics
} // namespace galaxy