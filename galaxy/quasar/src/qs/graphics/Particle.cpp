///
/// Particle.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Particle.hpp"

///
/// Core namespace.
///
namespace qs
{
	Particle::Particle()
	    : m_velocity {1.0f, 1.0f}
	{
	}

	Particle::Particle(const float x_vel, const float y_vel)
	    : m_velocity {x_vel, y_vel}
	{
	}

	Particle::Particle(Particle&& p)
	{
		this->m_velocity        = std::move(p.m_velocity);
		this->m_instance_buffer = std::move(p.m_instance_buffer);
	}

	Particle& Particle::operator=(Particle&& p)
	{
		if (this != &p)
		{
			this->m_velocity        = std::move(p.m_velocity);
			this->m_instance_buffer = std::move(p.m_instance_buffer);
		}

		return *this;
	}

	void Particle::set_velocity(const float x_vel, const float y_vel)
	{
		m_velocity.x = x_vel;
		m_velocity.y = y_vel;
	}

	void Particle::set_instance(std::span<glm::vec2> instances)
	{
		m_instance_buffer.create(instances, 1);
		m_va.set_instanced(m_instance_buffer);
	}

	const glm::vec2& Particle::velocity() const noexcept
	{
		return m_velocity;
	}
} // namespace qs