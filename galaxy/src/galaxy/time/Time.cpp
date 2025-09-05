///
/// Time.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Time.hpp"

namespace galaxy
{
	namespace time
	{
		static double s_delta_time = 0.0;

		auto now() noexcept -> std::chrono::local_time<std::chrono::system_clock::duration>
		{
			return std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
		}

		void dt(const double dt) noexcept
		{
			s_delta_time = dt;
		}

		double dt() noexcept
		{
			return s_delta_time;
		}
	} // namespace time
} // namespace galaxy
