///
/// Animated.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "Animated.hpp"

///
/// Core namespace.
///
namespace qs
{
	Animated::Animated()
	    : m_active_anim {nullptr}, m_paused {true}, m_time_spent_on_frame {0}
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
			PL_LOG(PL_FATAL, "Failed to set animation before playing.");
		}
	}

	void Animated::play(std::string_view animation)
	{
		m_paused = false;
		set_animation(animation);
	}

	void Animated::pause() noexcept
	{
		m_paused = true;
	}

	void Animated::stop() noexcept
	{
		if (m_active_anim != nullptr)
		{
			m_time_spent_on_frame = 0;
			m_paused              = true;

			m_active_anim->restart();
		}
	}
} // namespace qs