///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <chrono>
#include <fstream>

#include <nlohmann/json.hpp>

#include "Log.hpp"

using namespace std::chrono_literals;

namespace galaxy
{
	namespace error
	{
		Log& Log::handle() noexcept
		{
			static Log s_inst;
			return s_inst;
		}

		void Log::start() noexcept
		{
			static constexpr const auto path = "assets/log_settings.json";

			if (!std::filesystem::exists(path))
			{
				GALAXY_LOG(GALAXY_INFO, "Missing default logging config, recreating...");

				std::ofstream ofs;
				ofs.open(path, std::ofstream::out | std::ofstream::trunc);
				if (ofs.good())
				{
					ofs << nlohmann::json::parse("{\"min-level\": \"INFO\"}").dump(4);
					ofs.close();
				}
				else
				{
					ofs.close();
					GALAXY_LOG(GALAXY_FATAL, "Failed to save default logging config to disk.");
				}

				m_min_level = LogLevel::INFO;
			}
			else
			{
				nlohmann::json json;
				std::ifstream input;

				input.open(path, std::ifstream::in);
				if (!input.good())
				{
					input.close();
					GALAXY_LOG(GALAXY_FATAL, "Failed to open: logging config.");
				}
				else
				{
					input >> json;
					input.close();

					auto opt = magic_enum::enum_cast<LogLevel>(json.at("min-level").get<std::string>());
					if (!opt.has_value())
					{
						GALAXY_LOG(GALAXY_FATAL, "Failed to parse min log level. Must be an enum name.");
					}
					else
					{
						m_min_level = opt.value();
					}
				}
			}

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