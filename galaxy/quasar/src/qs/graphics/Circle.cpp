///
/// Circle.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include "Circle.hpp"

///
/// Core namespace.
///
namespace qs
{
	Circle::Circle() noexcept
		:m_thickness(1.0f)
	{
	}

	Circle::Circle(const float x, const float y, const float radius, const int fragments, protostar::Colour& colour) noexcept
		:m_thickness(1.0f)
	{
		create(x, y, radius, fragments, colour);
	}

	void Circle::create(const float x, const float y, const float radius, const int fragments, protostar::Colour& colour) noexcept
	{
		// Thanks to https://stackoverflow.com/a/33859443.
		// For help with maths.

		std::vector<qs::PrimitiveVertex> vertexs;
		IndexStorage indices;

		unsigned int count = 0;
		float increment = 2.0f * glm::pi<float>() / static_cast<float>(fragments);
		for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
		{
			vertexs.emplace_back(qs::PrimitiveVertex{ radius * glm::cos(angle) + x, radius * glm::sin(angle) + y, colour });
			indices.push_back(count);

			count++;
		}

		m_vertexBuffer.create<qs::PrimitiveVertex, qs::BufferTypeStatic>(vertexs);
		m_indexBuffer.create<qs::BufferTypeStatic>(indices);

		m_layout.add<qs::PrimitiveVertex, qs::VATypePosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VATypeColour>(4);

		m_vertexArray.create<qs::PrimitiveVertex>(m_vertexBuffer, m_indexBuffer, m_layout);
	}

	void Circle::bind() noexcept
	{
		m_vertexArray.bind();
		glLineWidth(m_thickness);
	}

	void Circle::unbind() noexcept
	{
		m_vertexArray.unbind();
	}

	void Circle::setThickness(const float thickness) noexcept
	{
		m_thickness = thickness;
	}
}