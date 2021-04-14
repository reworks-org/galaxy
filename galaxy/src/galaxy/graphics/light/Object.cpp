///
/// Object.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.hpp"

namespace galaxy
{
	namespace light
	{
		Object::Object() noexcept
		    : m_pos {0.0f, 0.0f, 0.0f}
		{
			glGenBuffers(1, &m_vbo);
			glGenVertexArrays(1, &m_vao);

			m_transform = glm::mat4 {1.0f};
		}

		Object::~Object() noexcept
		{
			glDeleteVertexArrays(1, &m_vao);
			glDeleteBuffers(1, &m_vbo);
		}

		void Object::create()
		{
			// clang-format off
            constexpr const float vertices[] = {
		        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
				0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
            };
			// clang-format on

			constexpr const auto verticies_size = sizeof(vertices);

			glBindVertexArray(m_vao);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, verticies_size, vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			m_transform = glm::translate(m_transform, m_pos);
			m_transform = glm::scale(m_transform, glm::vec3 {0.1f});
		}

		void Object::bind() noexcept
		{
			glBindVertexArray(m_vao);
		}

		void Object::unbind() noexcept
		{
			glBindVertexArray(0);
		}

		const glm::mat4& Object::get_transform() const noexcept
		{
			return m_transform;
		}

		const unsigned int Object::get_count() const noexcept
		{
			return 36;
		}
	} // namespace light
} // namespace galaxy