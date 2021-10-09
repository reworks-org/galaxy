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
		    : m_pos {0.0f, 0.0f, 0.0f}, m_normal {0.0f, 0.0f, 0.0f}, m_tangents {0.0f, 0.0f, 0.0f}, m_texels {0.0f, 0.0f}, m_index {0}
		{
		}

		void Vertex3D::set_index(const unsigned int index) noexcept
		{
			m_index = index;
		}

		const unsigned int Vertex3D::get_index() const noexcept
		{
			return m_index;
		}
	} // namespace graphics
} // namespace galaxy