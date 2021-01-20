///
/// Particle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "galaxy/algorithm/Random.hpp"

#include "Particle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Particle::Particle()
		    : m_life {1.0f}, m_angle {0.0f}, m_position {0.0f, 0.0f}, m_velocity {1.0f, 1.0f}
		{
			m_angle = glm::radians(algorithm::random(0.0f, 360.0f));
		}

		Particle::Particle(const float x_vel, const float y_vel)
		    : m_life {1.0f}, m_angle {0.0f}, m_position {0.0f, 0.0f}, m_velocity {x_vel, y_vel}
		{
			m_angle = glm::radians(algorithm::random(0.0f, 360.0f));
		}

		void Particle::set_position(const float x, const float y)
		{
			m_position.x = x;
			m_position.y = y;
		}

		void Particle::set_velocity(const float x_vel, const float y_vel)
		{
			m_velocity.x = x_vel;
			m_velocity.y = y_vel;
		}

		void Particle::move(const float dt)
		{
			m_position.x += (m_velocity.x * dt) * std::cos(m_angle);
			m_position.y += (m_velocity.y * dt) * std::sin(m_angle);
		}

		const glm::vec2& Particle::pos() const
		{
			return m_position;
		}

		const glm::vec2& Particle::velocity() const
		{
			return m_velocity;
		}
	} // namespace graphics
} // namespace galaxy