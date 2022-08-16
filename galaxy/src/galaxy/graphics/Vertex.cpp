///
/// Vertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		Vertex::Vertex() noexcept
			: m_pos {0.0f, 0.0f}
			, m_texels {0.0f, 0.0f}
		{
		}

		Vertex::Vertex(const glm::vec2& pos) noexcept
			: m_pos {pos}
			, m_texels {0.0f, 0.0f}
		{
		}

		Vertex::Vertex(const glm::vec2& pos, const glm::vec2& texels) noexcept
			: m_pos {pos}
			, m_texels {texels}
		{
		}

		Vertex::Vertex(const glm::vec2& pos, const graphics::Colour& colour) noexcept
			: m_pos {pos}
			, m_colour {colour}
		{
		}

		Vertex::Vertex(const glm::vec2& pos, const glm::vec2& texels, const graphics::Colour& colour) noexcept
			: m_pos {pos}
			, m_texels {texels}
			, m_colour {colour}
		{
		}

		Vertex::Vertex(Vertex&& v) noexcept
		{
			this->m_pos    = std::move(v.m_pos);
			this->m_texels = std::move(v.m_texels);
			this->m_colour = std::move(v.m_colour);
		}

		Vertex& Vertex::operator=(Vertex&& v) noexcept
		{
			if (this != &v)
			{
				this->m_pos    = std::move(v.m_pos);
				this->m_texels = std::move(v.m_texels);
				this->m_colour = std::move(v.m_colour);
			}

			return *this;
		}

		Vertex::Vertex(const Vertex& v) noexcept
		{
			this->m_pos    = v.m_pos;
			this->m_texels = v.m_texels;
			this->m_colour = v.m_colour;
		}

		Vertex& Vertex::operator=(const Vertex& v) noexcept
		{
			if (this != &v)
			{
				this->m_pos    = v.m_pos;
				this->m_texels = v.m_texels;
				this->m_colour = v.m_colour;
			}

			return *this;
		}

		std::array<Vertex, 4> Vertex::gen_quad_vertices(const float width, const float height) noexcept
		{
			// clang-format off
			std::array<graphics::Vertex, 4> vertices = 
			{
				graphics::Vertex {glm::vec2 {0.0f, 0.0f}, glm::vec2 {0.0f, 0.0f}},
				graphics::Vertex {glm::vec2 {width, 0.0f}, glm::vec2 {1.0f, 0.0f}},
				graphics::Vertex {glm::vec2 {width, height}, glm::vec2 {1.0f, 1.0f}},
				graphics::Vertex {glm::vec2 {0.0f, height}, glm::vec2 {0.0f, 1.0f}}
			};
			// clang-format on

			return vertices;
		}

		const std::array<unsigned int, 6>& Vertex::get_default_indices() noexcept
		{
			static const std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};
			return indices;
		}
	} // namespace graphics
} // namespace galaxy