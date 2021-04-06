///
/// Vertex3D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Vertex3D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Vertex3D::Vertex3D() noexcept
		    : m_pos {0.0f, 0.0f, 0.0f}, m_normal {0.0f, 0.0f, 0.0f}, m_texels {0.0f, 0.0f}
		{
		}

		Vertex3D::Vertex3D(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texels) noexcept
		    : m_pos {pos}, m_normal {normal}, m_texels {texels}
		{
		}
	} // namespace graphics
} // namespace galaxy