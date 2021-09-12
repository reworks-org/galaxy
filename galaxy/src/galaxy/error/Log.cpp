///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <chrono>

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log::Log()
			: m_min_level {LogLevel::INFO}
		{
		}

		Log& Log::handle() noexcept
		{
			static Log s_inst;
			return s_inst;
		}

		void Log::start() noexcept
		{
			m_run_thread = true;

			// clang-format off
			m_async = std::async(std::launch::async, [&]()
			{
				while (m_run_thread)
				{
					std::this_thread::sleep_for(5s);

					{
						std::lock_guard<std::mutex> lock { m_log_lock };

						while (!m_messages.empty())
						{
							for (const auto& sink : m_sinks)
							{
								sink->sink_message(m_messages.front());
							}

							m_messages.pop();
						}
					}
				}
			});
			// clang-format on
		}

		void Log::cleanup() noexcept
		{
			m_min_level  = LogLevel::INFO;
			m_run_thread = false;
			m_async.get();

			m_sinks.clear();
			m_messages = {};
		}
	} // namespace error
} // namespace galaxy