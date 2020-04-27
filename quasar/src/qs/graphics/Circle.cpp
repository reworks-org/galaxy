///
/// Circle.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include "Circle.hpp"

///
/// Core namespace.
///
namespace qs
{
	Circle::Circle() noexcept
		:m_count(0), m_thickness(1.0f), m_va(0), m_vb(0)
	{
	}

	Circle::Circle(const float x, const float y, const float radius, const int fragments) noexcept
		:m_count(0), m_thickness(1.0f), m_va(0), m_vb(0)
	{
		create(x, y, radius, fragments);
	}

	Circle::~Circle() noexcept
	{
		glDeleteVertexArrays(1, &m_va);
		glDeleteBuffers(1, &m_vb);
	}

	void Circle::create(const float x, const float y, const float radius, const int fragments) noexcept
	{
		// Thanks to https://stackoverflow.com/a/33859443.
		// For help with maths.

		glGenBuffers(1, &m_vb);
		glGenVertexArrays(1, &m_va);
		glBindVertexArray(m_va);

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);

		std::vector<glm::vec2> vertexs;
		float increment = 2.0f * glm::pi<float>() / static_cast<float>(fragments);

		for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
		{
			vertexs.emplace_back(radius * glm::cos(angle) + x, radius * glm::sin(angle) + y);
		}

		m_count = static_cast<unsigned int>(vertexs.size());
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_count, vertexs.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Circle::bind() noexcept
	{
		glBindVertexArray(m_va);
		glLineWidth(m_thickness);
	}

	void Circle::unbind() noexcept
	{
		glBindVertexArray(0);
	}

	void Circle::setThickness(const float thickness) noexcept
	{
		m_thickness = thickness;
	}

	const unsigned int Circle::getCount() const noexcept
	{
		return m_count;
	}
}