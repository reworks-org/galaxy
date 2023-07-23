///
/// Vertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vertex.hpp"

thread_local std::array<unsigned int, 6> default_indices = {0u, 1u, 3u, 1u, 2u, 3u};

namespace galaxy
{
	namespace graphics
	{
		Vertex::Vertex()
		{
		}

		Vertex::Vertex(const glm::vec2& pos, const glm::vec2& texels, const glm::vec2& normals, const graphics::Colour& colour)
			: m_pos {pos}
			, m_texels {texels}
			, m_colour {colour}
		{
		}

		Vertex::Vertex(Vertex&& v)
		{
			this->m_pos    = std::move(v.m_pos);
			this->m_texels = std::move(v.m_texels);
			this->m_colour = std::move(v.m_colour);
		}

		Vertex& Vertex::operator=(Vertex&& v)
		{
			if (this != &v)
			{
				this->m_pos    = std::move(v.m_pos);
				this->m_texels = std::move(v.m_texels);
				this->m_colour = std::move(v.m_colour);
			}

			return *this;
		}

		Vertex::Vertex(const Vertex& v)
		{
			this->m_pos    = v.m_pos;
			this->m_texels = v.m_texels;
			this->m_colour = v.m_colour;
		}

		Vertex& Vertex::operator=(const Vertex& v)
		{
			if (this != &v)
			{
				this->m_pos    = v.m_pos;
				this->m_texels = v.m_texels;
				this->m_colour = v.m_colour;
			}

			return *this;
		}

		std::array<Vertex, 4> Vertex::gen_quad_vertices(const int width, const int height)
		{
			// clang-format off
			std::array<graphics::Vertex, 4> vertices = 
			{
				graphics::Vertex {glm::vec2 {0.0f, 0.0f}, glm::vec2 {0.0f, 0.0f}},
				graphics::Vertex {glm::vec2 {static_cast<float>(width), 0.0f}, glm::vec2 {1.0f, 0.0f}},
				graphics::Vertex {glm::vec2 {static_cast<float>(width), static_cast<float>(height)}, glm::vec2 {1.0f, 1.0f}},
				graphics::Vertex {glm::vec2 {0.0f, static_cast<float>(height)}, glm::vec2 {0.0f, 1.0f}}
			};
			// clang-format on

			return vertices;
		}

		std::array<unsigned int, 6>& Vertex::get_default_indices()
		{
			return default_indices;
		}
	} // namespace graphics
} // namespace galaxy