///
/// AnimatedBatchSprite.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "AnimatedBatchSprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	AnimatedBatchSprite::AnimatedBatchSprite()
	    : BatchedSprite {}, Animated {}
	{
	}

	void AnimatedBatchSprite::update(const double dt)
	{
		if (!m_paused)
		{
			m_time_spent_on_frame += (dt * m_active_anim->get_speed());
			if (m_active_anim->get_current_frame()->get_time_per_frame() >= m_time_spent_on_frame)
			{
				m_time_spent_on_frame = 0;
				auto* new_frame       = m_active_anim->next_frame();

				update_region(new_frame->get_x(), new_frame->get_y(), 0.0f, 0.0f);
			}
		}
	}
} // namespace qs