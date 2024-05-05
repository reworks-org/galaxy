///
/// Ellipse.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#include "galaxy/meta/Memory.hpp"

#include "Ellipse.hpp"

namespace galaxy
{
	namespace graphics
	{
		Ellipse::Ellipse()
			: m_fragments {12.0f}
			, m_radii {10.0f, 5.0f}

		{
			m_mode = GL_LINE_LOOP;
		}

		Ellipse::Ellipse(Ellipse&& e)
			: Shape {std::move(e)}
		{
			this->m_radii     = std::move(e.m_radii);
			this->m_fragments = e.m_fragments;
		}

		Ellipse& Ellipse::operator=(Ellipse&& e)
		{
			if (this != &e)
			{
				Shape::operator=(std::move(e));

				this->m_radii     = std::move(e.m_radii);
				this->m_fragments = e.m_fragments;
			}

			return *this;
		}

		Ellipse::~Ellipse()
		{
		}

		void Ellipse::create(const float fragments, const glm::vec2& radii)
		{
			m_fragments = fragments;
			m_radii     = radii;

			// Thanks to https://stackoverflow.com/a/34735255
			// For help with maths.

			const auto theta  = 2.0f * glm::pi<float>() / m_fragments;
			const auto cosine = std::cosf(theta);
			const auto sine   = std::sinf(theta);

			auto temp = 0.0f;
			auto x    = 1.0f;
			auto y    = 0.0f;

			meta::vector<unsigned int>     indices;
			meta::vector<graphics::Vertex> vertices;

			auto count = 0u;
			for (auto i = 0; i < std::floor(m_fragments); i++)
			{
				graphics::Vertex vertex;
				vertex.m_pos.x = (x * m_radii.x) + m_radii.x;
				vertex.m_pos.y = (y * m_radii.y) + m_radii.y;

				vertices.emplace_back(vertex);
				indices.push_back(count);
				count++;

				temp = x;
				x    = cosine * x - sine * y;
				y    = sine * temp + cosine * y;
			}

			m_width  = m_radii.x * 2.0f;
			m_height = m_radii.y * 2.0f;
			m_vao.buffer(vertices, indices);
		}

		float Ellipse::fragments() const
		{
			return m_fragments;
		}

		const glm::vec2& Ellipse::radii() const
		{
			return m_radii;
		}
	} // namespace graphics
} // namespace galaxy
