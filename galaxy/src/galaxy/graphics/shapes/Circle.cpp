///
/// Circle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <vector>

#include <glad/glad.h>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

#include "Circle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Circle::Circle(const float x, const float y, const float radius, const unsigned int fragments, graphics::Colour& colour)
		{
			create(x, y, radius, fragments, colour);
		}

		Circle::Circle(const pr::Circle& circle, const unsigned int fragments, graphics::Colour& colour)
		{
			create(circle.get_x(), circle.get_y(), circle.radius(), fragments, colour);
		}

		void Circle::create(const float x, const float y, const float radius, const unsigned int fragments, graphics::Colour& colour)
		{
			// Thanks to https://stackoverflow.com/a/33859443.
			// For help with maths.

			std::vector<PrimitiveVertex> vertexs;
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

			m_vb.create<PrimitiveVertex, BufferStatic>(vertexs);
			m_ib.create<BufferStatic>(indices);

			m_layout.add<PrimitiveVertex, VAPosition>(2);
			m_layout.add<PrimitiveVertex, VAColour>(4);

			m_va.create<PrimitiveVertex>(m_vb, m_ib, m_layout);
		}

		void Circle::bind()
		{
			m_va.bind();
		}

		void Circle::unbind()
		{
			m_va.unbind();
		}

		const float Circle::get_x() const
		{
			return m_pos.x;
		}

		const float Circle::get_y() const
		{
			return m_pos.y;
		}

		const glm::vec2& Circle::get_xy() const
		{
			return m_pos;
		}

		const float Circle::radius() const
		{
			return m_radius;
		}
	} // namespace graphics
} // namespace galaxy