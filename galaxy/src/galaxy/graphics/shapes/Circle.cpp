///
/// Circle.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "galaxy/meta/Memory.hpp"

#include "Circle.hpp"

namespace galaxy
{
	namespace graphics
	{
		Circle::Circle()
			: m_fragments {20.0f}
			, m_radius {10.0f}

		{
			m_mode = GL_LINE_LOOP;
		}

		Circle::Circle(Circle&& c)
			: Shape {std::move(c)}
		{
			this->m_radius    = c.m_radius;
			this->m_fragments = c.m_fragments;
		}

		Circle& Circle::operator=(Circle&& c)
		{
			if (this != &c)
			{
				Shape::operator=(std::move(c));

				this->m_radius    = c.m_radius;
				this->m_fragments = c.m_fragments;
			}

			return *this;
		}

		Circle::~Circle()
		{
		}

		void Circle::create(const float fragments, const float radius)
		{
			m_fragments = fragments;
			m_radius    = radius;

			// Thanks to https://stackoverflow.com/a/33859443
			// For help with maths.

			auto       count     = 0u;
			const auto increment = (2.0f * glm::pi<float>()) / m_fragments;

			meta::vector<unsigned int>     indices;
			meta::vector<graphics::Vertex> vertices;

			for (float angle = 0.0f; angle <= (2.0f * glm::pi<float>()); angle += increment)
			{
				graphics::Vertex vertex;
				vertex.m_pos.x = (m_radius * glm::cos(angle)) + m_radius;
				vertex.m_pos.y = (m_radius * glm::sin(angle) + m_radius);

				vertices.emplace_back(vertex);
				indices.push_back(count);

				count++;
			}

			m_width  = m_radius * 2.0f;
			m_height = m_radius * 2.0f;
			m_vao.buffer(vertices, indices);
		}

		float Circle::fragments() const
		{
			return m_fragments;
		}

		float Circle::radius() const
		{
			return m_radius;
		}
	} // namespace graphics
} // namespace galaxy
