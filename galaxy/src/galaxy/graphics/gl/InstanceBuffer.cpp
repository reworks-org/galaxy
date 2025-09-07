///
/// InstanceBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "InstanceBuffer.hpp"

namespace galaxy
{
	namespace graphics
	{

		InstanceBuffer::InstanceBuffer()
			: m_id {0}
			, m_amount {0}
		{
			glCreateBuffers(1, &m_id);
		}

		InstanceBuffer::InstanceBuffer(InstanceBuffer&& i)
		{
			this->m_id     = i.m_id;
			this->m_amount = i.m_amount;

			i.m_id = 0;
		}

		InstanceBuffer& InstanceBuffer::operator=(InstanceBuffer&& i)
		{
			if (this != &i)
			{
				this->m_id     = i.m_id;
				this->m_amount = i.m_amount;

				i.m_id = 0;
			}

			return *this;
		}

		InstanceBuffer::~InstanceBuffer()
		{
			if (m_id != 0)
			{
				glDeleteBuffers(1, &m_id);
			}
		}

		void InstanceBuffer::buffer(std::span<glm::mat4> transforms)
		{
			m_amount = static_cast<int>(transforms.size());
			glNamedBufferData(m_id, transforms.size_bytes(), transforms.data(), GL_DYNAMIC_DRAW);
		}

		void InstanceBuffer::sub_buffer(const unsigned int index, std::span<glm::mat4> transforms)
		{
			glNamedBufferSubData(m_id, index * sizeof(glm::mat4), transforms.size_bytes(), transforms.data());
		}

		void InstanceBuffer::clear()
		{
			glNamedBufferData(m_id, m_amount * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
		}

		int InstanceBuffer::amount() const
		{
			return m_amount;
		}

		unsigned int InstanceBuffer::id() const
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy
