///
/// Vertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "Vertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		Vertex::Vertex() noexcept
		    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_colour {0.0f, 0.0f, 0.0f, 1.0f}, m_depth {0}
		{
		}

		Vertex::Vertex(const glm::vec2& pos, const int depth, const glm::vec2& texels, const graphics::Colour& colour) noexcept
		    : m_pos {pos}, m_texels {texels}
		{
			m_colour = const_cast<graphics::Colour&>(colour).normalized();

			set_depth(depth);
		}

		Vertex::Vertex(const glm::vec2& pos, const int depth, const graphics::Colour& colour, const glm::vec2& texels) noexcept
		    : m_pos {pos}, m_texels {texels}
		{
			m_colour = const_cast<graphics::Colour&>(colour).normalized();

			set_depth(depth);
		}

		Vertex::Vertex(Vertex&& v) noexcept
		{
			this->m_colour = std::move(v.m_colour);
			this->m_pos    = std::move(v.m_pos);
			this->m_texels = std::move(v.m_texels);
			this->m_depth  = v.m_depth;
		}

		Vertex& Vertex::operator=(Vertex&& v) noexcept
		{
			if (this != &v)
			{
				this->m_colour = std::move(v.m_colour);
				this->m_pos    = std::move(v.m_pos);
				this->m_texels = std::move(v.m_texels);
				this->m_depth  = v.m_depth;
			}

			return *this;
		}

		Vertex::Vertex(const Vertex& v) noexcept
		{
			this->m_colour = v.m_colour;
			this->m_pos    = v.m_pos;
			this->m_texels = v.m_texels;
			this->m_depth  = v.m_depth;
		}

		Vertex& Vertex::operator=(const Vertex& v) noexcept
		{
			if (this != &v)
			{
				this->m_colour = v.m_colour;
				this->m_pos    = v.m_pos;
				this->m_texels = v.m_texels;
				this->m_depth  = v.m_depth;
			}

			return *this;
		}

		void Vertex::set_colour(const graphics::Colour& colour) noexcept
		{
			m_colour = const_cast<graphics::Colour&>(colour).normalized();
		}

		void Vertex::set_depth(const int depth) noexcept
		{
			m_depth = std::clamp(depth, 0, 1000);
		}

		const glm::vec4& Vertex::get_colour() const noexcept
		{
			return m_colour;
		}

		const int Vertex::get_depth() const noexcept
		{
			return m_depth;
		}
	} // namespace graphics
} // namespace galaxy