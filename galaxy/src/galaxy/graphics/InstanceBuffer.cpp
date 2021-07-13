///
/// InstanceBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/error/Log.hpp"

#include "InstanceBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		InstanceBuffer::InstanceBuffer() noexcept
		    : m_ibo {0}
		{
			glGenBuffers(1, &m_ibo);
		}

		InstanceBuffer::InstanceBuffer(InstanceBuffer&& ib) noexcept
		{
			this->m_ibo            = ib.m_ibo;
			this->m_instance_count = ib.m_instance_count;

			ib.m_ibo = 0;
		}

		InstanceBuffer& InstanceBuffer::operator=(InstanceBuffer&& ib) noexcept
		{
			if (this != &ib)
			{
				this->m_ibo            = ib.m_ibo;
				this->m_instance_count = ib.m_instance_count;

				ib.m_ibo = 0;
			}

			return *this;
		}

		InstanceBuffer::~InstanceBuffer() noexcept
		{
			destroy();
		}

		void InstanceBuffer::create(std::span<glm::vec2> vertices)
		{
			if (!vertices.empty())
			{
				m_instance_count = vertices.size();

				glBindBuffer(GL_ARRAY_BUFFER, m_ibo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to create instance buffer with no instance data.");
			}
		}

		void InstanceBuffer::bind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ibo);
		}

		void InstanceBuffer::unbind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void InstanceBuffer::destroy() noexcept
		{
			glDeleteBuffers(1, &m_ibo);
			m_ibo = 0;
		}

		const unsigned int InstanceBuffer::id() const noexcept
		{
			return m_ibo;
		}

		const unsigned int InstanceBuffer::instance_count() const noexcept
		{
			return m_instance_count;
		}
	} // namespace graphics
} // namespace galaxy