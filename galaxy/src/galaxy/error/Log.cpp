///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <execution>
#include <filesystem>
#include <iostream>

#include "galaxy/platform/Platform.hpp"

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log::Log() noexcept
		    : m_min_level {0}
		{
			std::ios::sync_with_stdio(false);
			platform::configure_terminal();
		}

		Log& Log::get() noexcept
		{
			static Log s_inst;
			return s_inst;
		}

		void Log::start(std::string_view log_file)
		{
			auto path = std::filesystem::path {log_file};
			m_file_stream.open(path.string(), std::ofstream::out);

			// clang-format off
			m_thread = std::jthread([&]() {
				std::mutex mutex;
				std::condition_variable cv;

				while (m_file_stream.is_open())
				{
					std::unique_lock<std::mutex> lock {mutex};
					cv.wait(lock, [&]() {
						return !m_message.empty();
					});

					m_msg_mutex.lock();
					
					std::cout << m_message;
					m_file_stream << m_message;
					m_message.clear();

					m_msg_mutex.unlock();
				}
			});
			// clang-format on
		}

		void Log::finish()
		{
			m_file_stream.close();
			m_thread.request_stop();
			m_thread.join();
		}
	} // namespace error
} // namespace galaxy