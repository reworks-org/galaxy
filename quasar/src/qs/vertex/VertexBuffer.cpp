///
/// VertexBuffer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "VertexBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	VertexBuffer::VertexBuffer() noexcept
		:m_id(0)
	{
		glGenBuffers(1, &m_id);
	}

	VertexBuffer::~VertexBuffer() noexcept
	{
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::create(const VertexStorage& vertexs, const qs::BufferType bufferType, const unsigned int quadCount)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		if (bufferType == qs::BufferType::STATIC)
		{
			m_vertexStorage = vertexs;
			glBufferData(GL_ARRAY_BUFFER, m_vertexStorage.size() * sizeof(qs::Vertex), m_vertexStorage.data(), GL_STATIC_DRAW);
		}
		else if (bufferType == qs::BufferType::DYNAMIC)
		{
			m_vertexStorage = vertexs;
			glBufferData(GL_ARRAY_BUFFER, (quadCount * 4) * sizeof(qs::Vertex), nullptr, GL_DYNAMIC_DRAW);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::bind() noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() noexcept
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	const std::vector<qs::Vertex>& VertexBuffer::getVertexs() noexcept
	{
		return m_vertexStorage;
	}
}