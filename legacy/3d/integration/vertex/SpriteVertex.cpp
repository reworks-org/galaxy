///
/// SpriteVertex.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SpriteVertex.hpp"

namespace galaxy
{
	namespace graphics
	{
		SpriteVertex::SpriteVertex() noexcept
		    : m_pos {0.0f, 0.0f}, m_texels {0.0f, 0.0f}
		{
		}

		SpriteVertex::SpriteVertex(const float x, const float y, const float u, const float v) noexcept
		    : m_pos {x, y}, m_texels {u, v}

		{
		}

		SpriteVertex::SpriteVertex(const glm::vec2& pos, const glm::vec2& texels) noexcept
		    : m_pos {pos}, m_texels {texels}
		{
		}
	} // namespace graphics
} // namespace galaxy