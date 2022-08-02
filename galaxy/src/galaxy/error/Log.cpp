///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <BS_thread_pool.hpp>

#include "galaxy/core/ServiceLocator.hpp"

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log::Log() noexcept
			: m_min_level {LogLevel::INFO}
			, m_sync {0}
		{
		}

		Log& Log::handle() noexcept
		{
			static Log s_instance;
			return s_instance;
		}

		void Log::start() noexcept
		{
			m_run_thread = true;

			auto& tp = core::ServiceLocator<BS::thread_pool>::ref();
			tp.push_task([this]() {
				while (m_run_thread)
				{
					m_sync.acquire();

					while (!m_messages.empty())
					{
						for (const auto& sink : m_sinks)
						{
							sink->sink_message(m_messages.front());
						}

						m_messages.pop();
					}
				}
			});
		}

		void Log::finish()
		{
			m_min_level  = LogLevel::INFO;
			m_run_thread = false;

			m_sinks.clear();
			m_messages = {};
		}
	} // namespace error
} // namespace galaxy