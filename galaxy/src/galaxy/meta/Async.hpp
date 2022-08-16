///
/// Async.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_META_ASYNC_HPP_
#define GALAXY_META_ASYNC_HPP_

#include <future>

using namespace std::chrono_literals;

namespace galaxy
{
	namespace meta
	{
		///
		/// Check if an async thread has finished or not.
		///
		/// \tparam T Template type for std::future<T>.
		///
		/// \return Returns true when async thread has finished.
		///
		template<typename T = void>
		[[nodiscard]] inline bool is_work_done(std::future<T>& t)
		{
			return t.wait_for(0s) == std::future_status::ready;
		}
	} // namespace meta
} // namespace galaxy

#endif