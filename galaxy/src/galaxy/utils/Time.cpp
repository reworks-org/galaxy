///
/// Time.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Time.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace time
	{
		std::chrono::local_time<std::chrono::system_clock::duration> now() noexcept
		{
			return std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
		}
	} // namespace time
} // namespace galaxy
