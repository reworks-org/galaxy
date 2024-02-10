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
		Log::Log()
			: m_queue {10}
			, m_min_level {LogLevel::INFO}
		{
			m_sinks.reserve(2);
		}

		Log::~Log()
		{
			// Ensure thread isnt left running.
			m_active = false;
		}

		Log& Log::handle()
		{
			static Log s_instance;
			return s_instance;
		}

		void Log::init()
		{
			m_active    = true;
			m_min_level = LogLevel::INFO;

			auto& tp = core::ServiceLocator<BS::thread_pool>::ref();
			tp.detach_task([this]() {
				LogMessage message;
				while (m_active || m_queue.peek() != nullptr)
				{
					if (m_queue.try_dequeue(message))
					{
						for (const auto& sink : m_sinks)
						{
							sink->sink_message(message);
						}
					}

					// Prevent CPU going nuts.
					std::this_thread::sleep_for(0.1ms);
				}

				// When finished, remove sinks.
				m_sinks.clear();
			});
		}

		void Log::finish()
		{
			m_active = false;
		}
	} // namespace error
} // namespace galaxy
