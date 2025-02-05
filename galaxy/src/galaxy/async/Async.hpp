///
/// Async.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ASYNC_ASYNC_HPP_
#define GALAXY_ASYNC_ASYNC_HPP_

#include <chrono>
#include <future>

using namespace std::chrono_literals;

namespace galaxy
{
	namespace async
	{
		///
		/// Check if an async thread has finished or not.
		///
		/// \tparam T Template type for std::future<T>.
		///
		/// \return Returns true when async thread has finished.
		///
		template<typename T = void>
		[[nodiscard]]
		inline bool is_work_done(const std::future<T>& t) noexcept
		{
			return t.wait_for(0s) == std::future_status::ready;
		}
	} // namespace async
} // namespace galaxy

#endif
