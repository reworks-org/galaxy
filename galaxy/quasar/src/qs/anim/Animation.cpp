///
/// Animation.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Animation.hpp"

///
/// Core namespace.
///
namespace qs
{
	Animation::Animation()
	    : m_name {"null"}, m_looping {false}, m_speed {0.0f}, m_total_frames {0}, m_current_frame {0}
	{
	}

	Animation::Animation(std::string_view name, bool looping, pr::positive_float auto speed, std::span<qs::Frame> frames)
	    : m_name {name}, m_looping {looping}, m_speed {speed}, m_total_frames {frames.size()}, m_current_frame {0}, m_frames {frames.begin(), frames.end()}
	{
	}

	Animation::~Animation() noexcept
	{
		m_frames.clear();
	}
} // namespace qs