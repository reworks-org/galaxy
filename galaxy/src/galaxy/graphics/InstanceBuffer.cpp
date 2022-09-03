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
			, m_instance_count {0}
		{
			glCreateBuffers(1, &m_ibo);
		}

		InstanceBuffer::InstanceBuffer(InstanceBuffer&& ib) noexcept
			: m_ibo {0}
		{
			this->destroy();

			this->m_ibo            = ib.m_ibo;
			this->m_instance_count = ib.m_instance_count;

			ib.m_ibo = 0;
		}

		InstanceBuffer& InstanceBuffer::operator=(InstanceBuffer&& ib) noexcept
		{
			if (this != &ib)
			{
				this->destroy();

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
				m_instance_count = static_cast<unsigned int>(vertices.size());
				glNamedBufferData(m_ibo, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to upload empty instance buffer.");
			}
		}

		void InstanceBuffer::clear() noexcept
		{
			glInvalidateBufferData(m_ibo);
		}

		void InstanceBuffer::destroy() noexcept
		{
			if (m_ibo != 0)
			{
				glDeleteBuffers(1, &m_ibo);
				m_ibo = 0;
			}
		}

		unsigned int InstanceBuffer::id() const noexcept
		{
			return m_ibo;
		}

		unsigned int InstanceBuffer::instance_count() const noexcept
		{
			return m_instance_count;
		}
	} // namespace graphics
} // namespace galaxy