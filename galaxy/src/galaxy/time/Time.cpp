///
/// Time.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_timer.h>

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

		std::uint64_t time_since_epoch() noexcept
		{
			const auto duration = now().time_since_epoch();
			return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
		}

		void dt(const double dt) noexcept
		{
			s_delta_time = dt;
		}

		double dt() noexcept
		{
			return s_delta_time;
		}

		void delay(const int milli) noexcept
		{
			SDL_DelayPrecise(SDL_MS_TO_NS(milli));
		}

		std::uint64_t ms_to_ns(const std::uint64_t ms) noexcept
		{
			return SDL_MS_TO_NS(ms);
		}

		std::uint64_t ns_to_ms(const std::uint64_t ns) noexcept
		{
			return SDL_NS_TO_MS(ns);
		}

		std::uint64_t ns_to_s(const std::uint64_t ns) noexcept
		{
			return SDL_NS_TO_SECONDS(ns);
		}

		std::uint64_t ns_to_us(const std::uint64_t ns) noexcept
		{
			return SDL_NS_TO_US(ns);
		}

		std::uint64_t s_to_ns(const std::uint64_t s) noexcept
		{
			return SDL_SECONDS_TO_NS(s);
		}

		std::uint64_t us_to_ns(const std::uint64_t us) noexcept
		{
			return SDL_US_TO_NS(us);
		}
	} // namespace time
} // namespace galaxy
