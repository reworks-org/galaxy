///
/// ParticleInstance.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "ParticleInstance.hpp"

///
/// Core namespace.
///
namespace qs
{
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

	void ParticleInstance::set_instance(std::span<glm::vec3> instances)
	{
		m_instance_buffer.create(instances, 1);
		m_va.set_instanced(m_instance_buffer);
	}

	void ParticleInstance::update_instances(std::span<glm::vec3> instances)
	{
		m_instance_buffer.update(instances);
	}

	void ParticleInstance::bind()
	{
		m_va.bind();
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}

	void ParticleInstance::unbind()
	{
		m_va.unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace qs