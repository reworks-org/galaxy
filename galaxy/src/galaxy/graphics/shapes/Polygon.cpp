///
/// Polygon.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Polygon.hpp"

namespace galaxy
{
	namespace graphics
	{
		Polygon::Polygon()
		{
			m_mode   = GL_LINE_LOOP;
			m_width  = 0.0f;
			m_height = 0.0f;
		}

		Polygon::Polygon(Polygon&& p)
			: Shape {std::move(p)}
		{
			this->m_points = std::move(p.m_points);
		}

		Polygon& Polygon::operator=(Polygon&& p)
		{
			if (this != &p)
			{
				Shape::operator=(std::move(p));
				this->m_points = std::move(p.m_points);
			}

			return *this;
		}

		Polygon::~Polygon()
		{
		}

		void Polygon::create(const meta::vector<glm::vec2>& points)
		{
			m_points = points;

			meta::vector<unsigned int>     indices;
			meta::vector<graphics::Vertex> vertices;

			auto count = 0u;
			for (const auto& point : m_points)
			{
				if (point.x > m_width)
				{
					m_width = point.x;
				}

				if (point.y > m_height)
				{
					m_height = point.y;
				}

				graphics::Vertex vertex;
				vertex.m_pos = point;

				vertices.emplace_back(vertex);
				indices.push_back(count);

				count++;
			}

			m_vao.buffer(vertices, indices);
		}

		const meta::vector<glm::vec2>& Polygon::points() const
		{
			return m_points;
		}
	} // namespace graphics
} // namespace galaxy
