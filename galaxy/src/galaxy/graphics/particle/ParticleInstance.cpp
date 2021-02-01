///
/// ParticleInstance.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "ParticleInstance.hpp"

namespace galaxy
{
	namespace graphics
	{
		ParticleInstance::ParticleInstance(ParticleInstance&& p) noexcept
		    : VertexData {std::move(p)}, Texture {std::move(p)}
		{
			this->m_instance_buffer = std::move(p.m_instance_buffer);
		}

		ParticleInstance& ParticleInstance::operator=(ParticleInstance&& p) noexcept
		{
			if (this != &p)
			{
				VertexData::operator=(std::move(p));

				Texture::operator=(std::move(p));

				this->m_instance_buffer = std::move(p.m_instance_buffer);
			}

			return *this;
		}

		void ParticleInstance::create(const float tex_x, const float tex_y)
		{
			std::vector<SpriteVertex> vb_arr;
			vb_arr.emplace_back(0.0f, 0.0f, tex_x, tex_y);
			vb_arr.emplace_back(0.0f + m_width, 0.0f, tex_x + m_width, tex_y);
			vb_arr.emplace_back(0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height);
			vb_arr.emplace_back(0.0f, 0.0f + m_height, tex_x, tex_y + m_height);

			m_vb.create<SpriteVertex>(vb_arr);

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};
			m_ib.create(ib_arr);

			m_layout.add<SpriteVertex, meta::VAPosition>(2);
			m_layout.add<SpriteVertex, meta::VATexel>(2);

			m_va.create<SpriteVertex>(m_vb, m_ib, m_layout);
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
	} // namespace graphics
} // namespace galaxy