///
/// Time.cpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#include "Time.hpp"

///
/// Core namespace.
///
namespace pr
{
	Timer::Timer()
	    : m_repeat {false}, m_delay {0}
	{
	}

	Timer::~Timer()
	{
		stop();
	}

	void Timer::set_repeating(bool repeat)
	{
		std::lock_guard<std::mutex> lock {m_mutex};
		m_repeat = repeat;
	}

	void Timer::stop()
	{
		m_repeat = false;
		m_thread.request_stop();
		m_thread.join();
	}

	const double time_since_epoch()
	{
		// Return time since epoch using chrono.
		return std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
} // namespace pr