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

	void AnimatedSprite::update(const double dt)
	{
		if (!m_paused)
		{
			m_time_spent_on_frame += (dt * m_active_anim->get_speed());
			if (m_active_anim->get_current_frame()->get_time_per_frame() >= m_time_spent_on_frame)
			{
				m_time_spent_on_frame = 0;
				auto* new_frame       = m_active_anim->next_frame();

				auto vb             = m_vb.get<qs::SpriteVertex>();
				auto* vertex        = &vb[0];
				vertex->m_texels[0] = new_frame->get_x();
				vertex->m_texels[1] = new_frame->get_y();

				vertex              = &vb[1];
				vertex->m_texels[0] = new_frame->get_x() + m_width;
				vertex->m_texels[1] = new_frame->get_y();

				vertex              = &vb[2];
				vertex->m_texels[0] = new_frame->get_x() + m_width;
				vertex->m_texels[1] = new_frame->get_y() + m_height;

				vertex              = &vb[3];
				vertex->m_texels[0] = new_frame->get_x();
				vertex->m_texels[1] = new_frame->get_y() + m_height;

				glNamedBufferSubData(m_vb.id(), 0, vb.size() * sizeof(qs::SpriteVertex), vb.data());
			}
		}
	}
} // namespace qs