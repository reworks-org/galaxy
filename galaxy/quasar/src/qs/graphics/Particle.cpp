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
	    : m_life {100.0f}, m_position {0.0f, 0.0f}, m_velocity {1.0f, 1.0f}
	{
	}

	Particle::Particle(const float x_vel, const float y_vel)
	    : m_life {100.0f}, m_position {0.0f, 0.0f}, m_velocity {x_vel, y_vel}
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
		m_position.x += (m_velocity.x * dt);
		m_position.y += (m_velocity.y * dt);
	}

	const glm::vec2& Particle::pos() const noexcept
	{
		return m_position;
	}

	const glm::vec2& Particle::velocity() const noexcept
	{
		return m_velocity;
	}

	ParticleInstance::ParticleInstance(ParticleInstance&& p)
	{
		this->m_instance_buffer = std::move(p.m_instance_buffer);
	}

	ParticleInstance& ParticleInstance::operator=(ParticleInstance&& p)
	{
		if (this != &p)
		{
			this->m_instance_buffer = std::move(p.m_instance_buffer);
		}

		return *this;
	}

	void ParticleInstance::set_opacity(const float opacity) noexcept
	{
		if (m_opacity > 1.0f)
		{
			m_opacity = 1.0f;
		}
		else if (m_opacity < 0.0f)
		{
			m_opacity = 0.0f;
		}
		else
		{
			m_opacity = opacity;
		}
	}

	void ParticleInstance::set_instance(std::span<glm::vec2> instances)
	{
		m_instance_buffer.create(instances, 1);
		m_va.set_instanced(m_instance_buffer);
	}

	void ParticleInstance::update_instances(std::span<glm::vec2> instances)
	{
		m_instance_buffer.update(instances);
	}

	void ParticleInstance::bind() noexcept
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void ParticleInstance::unbind() noexcept
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	const float ParticleInstance::opacity() const noexcept
	{
		return m_opacity;
	}
} // namespace qs