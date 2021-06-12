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
		    : m_id {0}, m_divisor {0}
		{
			glGenBuffers(1, &m_id);
		}

		InstanceBuffer::InstanceBuffer(InstanceBuffer&& vb) noexcept
		{
			this->m_id      = vb.m_id;
			this->m_divisor = vb.m_divisor;

			vb.m_id = 0;
		}

		InstanceBuffer& InstanceBuffer::operator=(InstanceBuffer&& vb) noexcept
		{
			if (this != &vb)
			{
				this->m_id      = vb.m_id;
				this->m_divisor = vb.m_divisor;

				vb.m_id = 0;
			}

			return *this;
		}

		InstanceBuffer::~InstanceBuffer() noexcept
		{
			glDeleteBuffers(1, &m_id);
		}

		void InstanceBuffer::create(std::span<glm::vec3> offsets, unsigned int divisor)
		{
			m_divisor = divisor;
			bind();

			if (!offsets.empty())
			{
				glBufferData(GL_ARRAY_BUFFER, offsets.size_bytes(), offsets.data(), GL_DYNAMIC_DRAW);
			}
			else
			{
				glBufferData(GL_ARRAY_BUFFER, 4, nullptr, GL_DYNAMIC_DRAW);
			}

			unbind();
		}

		void InstanceBuffer::update(std::span<glm::vec3> offsets)
		{
			bind();

			glInvalidateBufferData(m_id);
			glBufferData(GL_ARRAY_BUFFER, offsets.size_bytes(), offsets.data(), GL_DYNAMIC_DRAW);

			unbind();
		}

		void InstanceBuffer::bind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_id);
		}

		void InstanceBuffer::unbind() noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		const unsigned int InstanceBuffer::id() const noexcept
		{
			return m_id;
		}

		const unsigned int InstanceBuffer::divisor() const noexcept
		{
			return m_divisor;
		}
	} // namespace graphics
} // namespace galaxy