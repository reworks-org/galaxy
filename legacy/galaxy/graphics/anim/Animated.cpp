///
/// Animated.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"

#include "Animated.hpp"

namespace galaxy
{
	namespace graphics
	{
		Animated::Animated()
		    : m_active_anim {nullptr}, m_paused {true}, m_time_spent_on_frame {0.0}
		{
		}

		Animated::~Animated()
		{
			m_animations.clear();
		}

		void Animated::set_animation(std::string_view animation)
		{
			// Reset current anim first.
			if (m_active_anim != nullptr)
			{
				m_active_anim->restart();
			}

			// Then update to new anim.
			m_active_anim = &m_animations[static_cast<std::string>(animation)];
			m_active_anim->restart();
		}

		void Animated::play()
		{
			if (m_active_anim != nullptr)
			{
				m_paused = false;
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to set animation before playing.");
			}
		}

		void Animated::play(std::string_view animation)
		{
			m_paused = false;
			set_animation(animation);
		}

		void Animated::pause()
		{
			m_paused = true;
		}

		void Animated::stop()
		{
			if (m_active_anim != nullptr)
			{
				m_time_spent_on_frame = 0.0;
				m_paused              = true;

				m_active_anim->restart();
			}
		}

		Animation* Animated::get_cur_animation()
		{
			return m_active_anim;
		}

		auto Animated::get_all_anims() -> const robin_hood::unordered_map<std::string, Animation>&
		{
			return m_animations;
		}
	} // namespace graphics
} // namespace galaxy