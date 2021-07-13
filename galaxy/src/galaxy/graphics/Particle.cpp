///
/// Particle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glm/trigonometric.hpp>

#include "galaxy/math/Random.hpp"

#include "Particle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Particle::Particle() noexcept
		    : m_life {1.0f}, m_offset {0.0f, 0.0f}, m_velocity {1.0f, 1.0f}
		{
			m_angle = glm::radians(math::random(0.0f, 360.0f));
		}

		Particle::Particle(Particle&& p) noexcept
		{
			this->m_angle    = p.m_angle;
			this->m_life     = p.m_life;
			this->m_offset   = std::move(p.m_offset);
			this->m_velocity = std::move(p.m_velocity);
		}

		Particle::Particle(const Particle& p) noexcept
		{
			this->m_angle    = p.m_angle;
			this->m_life     = p.m_life;
			this->m_offset   = p.m_offset;
			this->m_velocity = p.m_velocity;
		}

		Particle& Particle::operator=(Particle&& p) noexcept
		{
			if (this != &p)
			{
				this->m_angle    = p.m_angle;
				this->m_life     = p.m_life;
				this->m_offset   = std::move(p.m_offset);
				this->m_velocity = std::move(p.m_velocity);
			}

			return *this;
		}

		Particle& Particle::operator=(const Particle& p) noexcept
		{
			if (this != &p)
			{
				this->m_angle    = p.m_angle;
				this->m_life     = p.m_life;
				this->m_offset   = p.m_offset;
				this->m_velocity = p.m_velocity;
			}

			return *this;
		}

		void Particle::move(const float dt) noexcept
		{
			m_offset.x += (m_velocity.x * dt) * glm::cos(m_angle);
			m_offset.y += (m_velocity.y * dt) * glm::sin(m_angle);
		}
	} // namespace graphics
} // namespace galaxy