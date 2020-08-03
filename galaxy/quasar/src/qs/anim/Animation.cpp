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

	const std::string& Animation::get_name() noexcept
	{
		return m_name;
	}

	const bool Animation::is_looping() const noexcept
	{
		return m_looping;
	}

	const float Animation::get_speed() const noexcept
	{
		return m_speed;
	}

	const std::size_t Animation::get_total_frames() const noexcept
	{
		return m_total_frames;
	}

	const std::size_t Animation::get_current_frame() const noexcept
	{
		return m_current_frame;
	}

	const auto& Animation::get_frames() noexcept
	{
		return m_frames;
	}
} // namespace qs