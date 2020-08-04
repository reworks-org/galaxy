///
/// Circle.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glad/glad.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include "Circle.hpp"

///
/// Core namespace.
///
namespace qs
{
	Circle::Circle() noexcept
	    : m_thickness {1.0f}
	{
	}

	Circle::Circle(const float x, const float y, const pr::positive_float auto radius, const pr::positive_int auto fragments, const pr::Colour& colour, const pr::positive_float auto thickness)
	    : m_thickness {thickness}
	{
		create(x, y, radius, fragments, colour, thickness);
	}

	void Circle::create(const float x, const float y, const pr::positive_float auto radius, const pr::positive_int auto fragments, const pr::Colour& colour, const pr::positive_float auto thickness)
	{
		// Thanks to https://stackoverflow.com/a/33859443.
		// For help with maths.

		m_thickness = thickness;

		std::vector<qs::PrimitiveVertex> vertexs;
		std::vector<unsigned int> indices;

		unsigned int count        = 0;
		constexpr float incr_stat = 2.0f * glm::pi<float>();
		float increment           = incr_stat / static_cast<float>(fragments);
		for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
		{
			vertexs.emplace_back(radius * glm::cos(angle) + x, radius * glm::sin(angle) + y, colour);
			indices.push_back(count);

			count++;
		}

		m_vb.create<qs::PrimitiveVertex, qs::BufferStatic>(vertexs);
		m_ib.create<qs::BufferStatic>(indices);

		m_layout.add<qs::PrimitiveVertex, qs::VAPosition>(2);
		m_layout.add<qs::PrimitiveVertex, qs::VAColour>(4);

		m_va.create<qs::PrimitiveVertex>(m_vb, m_ib, m_layout);
	}

	void Circle::bind() noexcept
	{
		m_va.bind();
		glLineWidth(m_thickness);
	}

	void Circle::unbind() noexcept
	{
		m_va.unbind();
	}
} // namespace qs