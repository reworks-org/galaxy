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
		Particle::Particle() noexcept
		    : m_life {1.0f}, m_angle {0.0f}, m_position {0.0f, 0.0f}, m_velocity {1.0f, 1.0f}
		{
			m_angle = glm::radians(algorithm::random(0.0f, 360.0f));
		}

		Particle::Particle(const float x_vel, const float y_vel) noexcept
		    : m_life {1.0f}, m_angle {0.0f}, m_position {0.0f, 0.0f}, m_velocity {x_vel, y_vel}
		{
			m_angle = glm::radians(algorithm::random(0.0f, 360.0f));
		}

		Particle::Particle(const Particle& p) noexcept
		{
			this->m_angle    = p.m_angle;
			this->m_life     = p.m_life;
			this->m_position = p.m_position;
			this->m_velocity = p.m_velocity;
		}

		Particle::Particle(Particle&& p) noexcept
		{
			this->m_angle    = p.m_angle;
			this->m_life     = p.m_life;
			this->m_position = std::move(p.m_position);
			this->m_velocity = std::move(p.m_velocity);
		}

		Particle& Particle::operator=(const Particle& p) noexcept
		{
			if (this != &p)
			{
				this->m_angle    = p.m_angle;
				this->m_life     = p.m_life;
				this->m_position = p.m_position;
				this->m_velocity = p.m_velocity;
			}

			return *this;
		}

		Particle& Particle::operator=(Particle&& p) noexcept
		{
			if (this != &p)
			{
				this->m_angle    = p.m_angle;
				this->m_life     = p.m_life;
				this->m_position = std::move(p.m_position);
				this->m_velocity = std::move(p.m_velocity);
			}

			return *this;
		}

		void Particle::set_position(const float x, const float y) noexcept
		{
			m_position.x = x;
			m_position.y = y;
		}

		void Particle::set_velocity(const float x_vel, const float y_vel) noexcept
		{
			m_velocity.x = x_vel;
			m_velocity.y = y_vel;
		}

		void Particle::move(const float dt) noexcept
		{
			m_position.x += (m_velocity.x * dt) * std::cos(m_angle);
			m_position.y += (m_velocity.y * dt) * std::sin(m_angle);
		}

		const glm::vec2& Particle::pos() const noexcept
		{
			return m_position;
		}

		const glm::vec2& Particle::velocity() const noexcept
		{
			return m_velocity;
		}

		const float Particle::angle() const noexcept
		{
			return m_angle;
		}
	} // namespace graphics
} // namespace galaxy