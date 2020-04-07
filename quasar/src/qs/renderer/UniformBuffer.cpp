///
/// UniformBuffer.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "UniformBuffer.hpp"

///
/// Core namespace.
///
namespace qs
{
	UniformBuffer::UniformBuffer() noexcept
		:m_id(0)
	{
		glGenBuffers(1, &m_id);
		bind();
		//glBufferData(GL_UNIFORM_BUFFER, 152, nullptr, GL_STATIC_DRAW);
		//https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
	}

	UniformBuffer::~UniformBuffer() noexcept
	{
	}

	void UniformBuffer::bind() noexcept
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	}
}