///
/// AnimatedSprite.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "AnimatedSprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	AnimatedSprite::AnimatedSprite()
	    : Sprite {}, Animated {}
	{
	}

	void AnimatedSprite::update()
	{
		if (m_dirty_texels)
		{
			auto vb             = m_vb.get<qs::SpriteVertex>();
			auto* vertex        = &vb[0];
			vertex->m_texels[0] = m_texels.x;
			vertex->m_texels[1] = m_texels.y;

			vertex              = &vb[1];
			vertex->m_texels[0] = m_texels.x + m_width;
			vertex->m_texels[1] = m_texels.y;

			vertex              = &vb[2];
			vertex->m_texels[0] = m_texels.x + m_width;
			vertex->m_texels[1] = m_texels.y + m_height;

			vertex              = &vb[3];
			vertex->m_texels[0] = m_texels.x;
			vertex->m_texels[1] = m_texels.y + m_height;

			glNamedBufferSubData(m_vb.id(), 0, vb.size() * sizeof(qs::SpriteVertex), vb.data());
			m_dirty_texels = false;
		}
	}
} // namespace qs