///
/// Log.cpp
/// galaxy
///
/// See LICENSE.txt.
///

#include <chrono>

#include <nlohmann/json.hpp>

#include "galaxy/scripting/JSONUtils.hpp"

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
			if (!std::filesystem::exists("logs/log_settings.json"))
			{
				GALAXY_LOG(GALAXY_INFO, "Missing default logging config, recreating...");

				if (!json::save_to_disk("log_settings.json", nlohmann::json::parse("{\"min-level\": \"INFO\"}")))
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to create default logging config.");
				}

				m_min_level = LogLevel::INFO;
			}
			else
			{
				auto res = json::parse_from_disk("logs/log_settings.json");
				if (!res.has_value())
				{
					GALAXY_LOG(GALAXY_FATAL, "Failed to load logging config.");
				}
				else
				{
					auto opt = magic_enum::enum_cast<LogLevel>(res.value().at("min-level").get<std::string>());
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