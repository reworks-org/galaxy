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
	    : m_life {1.0f}, m_position {0.0f, 0.0f}, m_velocity {1.0f, 1.0f}
	{
	}

	Particle::Particle(const float x_vel, const float y_vel)
	    : m_life {1.0f}, m_position {0.0f, 0.0f}, m_velocity {x_vel, y_vel}
	{
	}

	Particle::Particle(const Particle& p)
	{
		this->m_life     = p.m_life;
		this->m_position = p.m_position;
		this->m_velocity = p.m_velocity;
	}

	Particle::Particle(Particle&& p)
	{
		this->m_life     = p.m_life;
		this->m_position = std::move(p.m_position);
		this->m_velocity = std::move(p.m_velocity);
	}

	Particle& Particle::operator=(const Particle& p)
	{
		this->m_life     = p.m_life;
		this->m_position = p.m_position;
		this->m_velocity = p.m_velocity;

		return *this;
	}

	Particle& Particle::operator=(Particle&& p)
	{
		if (this != &p)
		{
			this->m_life     = p.m_life;
			this->m_position = std::move(p.m_position);
			this->m_velocity = std::move(p.m_velocity);
		}

		return *this;
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
		//m_position.x += (m_velocity.x * dt);
		//m_position.y += (m_velocity.y * dt);

		m_position += m_velocity;
	}

	const glm::vec2& Particle::pos() const noexcept
	{
		return m_position;
	}

	const glm::vec2& Particle::velocity() const noexcept
	{
		return m_velocity;
	}
} // namespace qs