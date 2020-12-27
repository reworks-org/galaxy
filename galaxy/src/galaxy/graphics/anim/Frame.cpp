///
/// Frame.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Frame.hpp"

namespace galaxy
{
	namespace graphics
	{
		Frame::Frame()
		    : m_x {0.0f}, m_y {0.0f}, m_time_per_frame {0.0}
		{
		}

		Frame::Frame(const float x, const float y, const double time_per_frame)
		    : m_x {x}, m_y {y}, m_time_per_frame {time_per_frame}
		{
		}

		const float Frame::get_x() const
		{
			return m_x;
		}

		const float Frame::get_y() const
		{
			return m_y;
		}

		const double Frame::get_time_per_frame() const
		{
			return m_time_per_frame;
		}
	} // namespace graphics
} // namespace galaxy