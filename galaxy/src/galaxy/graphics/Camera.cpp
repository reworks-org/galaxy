///
/// Camera.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Camera.hpp"

namespace galaxy
{
	namespace graphics
	{
		Camera::Camera() noexcept
		    : m_forward_key {input::Keys::W},
		      m_back_key {input::Keys::S},
		      m_left_key {input::Keys::A},
		      m_right_key {input::Keys::D},
		      m_moving_fwd {false},
		      m_moving_back {false},
		      m_moving_left {false},
		      m_moving_right {false},
		      m_speed {1.0f}
		{
		}
	} // namespace graphics
} // namespace galaxy