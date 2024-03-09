///
/// Point.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "galaxy/meta/Memory.hpp"

#include "Point.hpp"

namespace galaxy
{
	namespace graphics
	{
		Point::Point()
		{
			m_mode = GL_POINTS;
		}

		Point::~Point()
		{
		}

		void Point::create(const glm::vec2& pos)
		{
			m_pos = pos;

			graphics::Vertex vertex;
			vertex.m_pos = m_pos;

			meta::vector<unsigned int>     indices  = {0u};
			meta::vector<graphics::Vertex> vertices = {vertex};

			m_vao.buffer(vertices, indices);
		}

		const glm::vec2& Point::pos() const
		{
			return m_pos;
		}
	} // namespace graphics
} // namespace galaxy
