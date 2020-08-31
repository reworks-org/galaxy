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

	Animated::Animated(const Animated& a)
	{
		this->m_active_anim         = a.m_active_anim;
		this->m_animations          = a.m_animations;
		this->m_paused              = a.m_paused;
		this->m_time_spent_on_frame = a.m_time_spent_on_frame;
	}

	Animated::Animated(Animated&& a)
	{
		this->m_active_anim         = a.m_active_anim;
		this->m_animations          = std::move(a.m_animations);
		this->m_paused              = a.m_paused;
		this->m_time_spent_on_frame = a.m_time_spent_on_frame;

		a.m_active_anim         = nullptr;
		a.m_paused              = true;
		a.m_time_spent_on_frame = 0;
	}

	Animated& Animated::operator=(const Animated& a)
	{
		this->m_active_anim         = a.m_active_anim;
		this->m_animations          = a.m_animations;
		this->m_paused              = a.m_paused;
		this->m_time_spent_on_frame = a.m_time_spent_on_frame;

		return *this;
	}

	Animated& Animated::operator=(Animated&& a)
	{
		if (this != &a)
		{
			this->m_active_anim         = a.m_active_anim;
			this->m_animations          = std::move(a.m_animations);
			this->m_paused              = a.m_paused;
			this->m_time_spent_on_frame = a.m_time_spent_on_frame;

			a.m_active_anim         = nullptr;
			a.m_paused              = true;
			a.m_time_spent_on_frame = 0;
		}

		return *this;
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

	qs::Animation* Animated::get_cur_animation() noexcept
	{
		return m_active_anim;
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