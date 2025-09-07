///
/// Polyline.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Polyline.hpp"

namespace galaxy
{
	namespace graphics
	{
		Polyline::Polyline()
		{
			m_mode   = GL_LINE_LOOP;
			m_width  = 0.0f;
			m_height = 0.0f;
		}

		Polyline::Polyline(Polyline&& p)
			: Shape {std::move(p)}
		{
			this->m_points = std::move(p.m_points);
		}

		Polyline& Polyline::operator=(Polyline&& p)
		{
			if (this != &p)
			{
				Shape::operator=(std::move(p));
				this->m_points = std::move(p.m_points);
			}

			return *this;
		}

		Polyline::~Polyline()
		{
		}

		void Polyline::create(const meta::vector<glm::vec2>& points)
		{
			m_points = points;

			meta::vector<unsigned int>     indices;
			meta::vector<graphics::Vertex> vertices;

			auto count = 0u;
			for (const auto& point : m_points)
			{
				graphics::Vertex vertex;
				vertex.m_pos = point;

				vertices.emplace_back(vertex);
				indices.push_back(count);

				count++;
			}

			m_vao.buffer(vertices, indices);
		}

		const meta::vector<glm::vec2>& Polyline::points() const
		{
			return m_points;
		}
	} // namespace graphics
} // namespace galaxy
