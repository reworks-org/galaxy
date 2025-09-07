///
/// Shape.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>

#include "Shape.hpp"

namespace galaxy
{
	namespace graphics
	{
		Shape::Shape()
			: m_mode {GL_TRIANGLES}
			, m_width {0.0f}
			, m_height {0.0f}
		{
		}

		Shape::Shape(Shape&& s)
		{
			this->m_vao    = std::move(s.m_vao);
			this->m_colour = std::move(s.m_colour);
			this->m_mode   = s.m_mode;
			this->m_width  = s.m_width;
			this->m_height = s.m_height;
		}

		Shape& Shape::operator=(Shape&& s)
		{
			if (this != &s)
			{
				this->m_vao    = std::move(s.m_vao);
				this->m_colour = std::move(s.m_colour);
				this->m_mode   = s.m_mode;
				this->m_width  = s.m_width;
				this->m_height = s.m_height;
			}

			return *this;
		}

		Shape::~Shape()
		{
		}

		unsigned int Shape::mode() const
		{
			return m_mode;
		}

		float Shape::width() const
		{
			return m_width;
		}

		float Shape::height() const
		{
			return m_height;
		}

		VertexArray& Shape::vao()
		{
			return m_vao;
		}
	} // namespace graphics
} // namespace galaxy
