///
/// Frame.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Frame.hpp"

///
/// Core namespace.
///
namespace qs
{
	Frame::Frame()
	    : m_x {0.0f}, m_y {0.0f}, m_time_per_frame {0.0}
	{
	}

	Frame::Frame(const float x, const float y, const double time_per_frame)
	    : m_x {x}, m_y {y}, m_time_per_frame {time_per_frame}
	{
	}

	Frame::Frame(const Frame& f)
	{
		this->m_x              = f.m_x;
		this->m_y              = f.m_y;
		this->m_time_per_frame = f.m_time_per_frame;
	}

	Frame::Frame(Frame&& f)
	{
		this->m_x              = f.m_x;
		this->m_y              = f.m_y;
		this->m_time_per_frame = f.m_time_per_frame;
	}

	Frame& Frame::operator=(const Frame& f)
	{
		this->m_x              = f.m_x;
		this->m_y              = f.m_y;
		this->m_time_per_frame = f.m_time_per_frame;

		return *this;
	}

	Frame& Frame::operator=(Frame&& f)
	{
		if (this != &f)
		{
			this->m_x              = f.m_x;
			this->m_y              = f.m_y;
			this->m_time_per_frame = f.m_time_per_frame;
		}

		return *this;
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
} // namespace qs