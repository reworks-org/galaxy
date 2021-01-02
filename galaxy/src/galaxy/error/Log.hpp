///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOG_HPP_
#define GALAXY_ERROR_LOG_HPP_

#include <fstream>
#include <mutex>
#include <thread>

#include <date/tz.h>
#include <fmt/format.h>

#include "galaxy/error/LogLevel.hpp"

// clang-format off

///
/// Log macro for start().
///
/// \param log_file Log file path.
///
#define GALAXY_LOG_START(log_file) galaxy::error::Log::get().start(log_file)

///
/// Log macro for start().
///
#define GALAXY_LOG_FINISH galaxy::error::Log::get().finish()

///
/// Static interface handle.
///
#define GALAXY_LOG_GET galaxy::error::Log::get()

///
/// INFO log level macro shortcut.
///
#define GALAXY_INFO galaxy::error::Info{}

///
/// DEBUG log level macro shortcut.
///
#define GALAXY_DEBUG galaxy::error::Debug{}

///
/// WARNING log level macro shortcut.
///
#define GALAXY_WARNING galaxy::error::Warning{}

///
/// ERROR log level macro shortcut.
///
#define GALAXY_ERROR galaxy::error::Error{}

///
/// FATAL log level macro shortcut.
///
#define GALAXY_FATAL galaxy::error::Fatal{}

///
/// Macro shortcut with variadic arguments.
///
/// \param level Log error level.
/// \param message Message to log.
///
#define GALAXY_LOG(...) galaxy::error::Log::get().log(__VA_ARGS__)

///
/// Enable testing mode.
///
#define GALAXY_ENABLE_TESTING_MODE galaxy::error::Log::get().set_testing(true)

///
/// Disable testing mode.
///
#define GALAXY_DISABLE_TESTING_MODE galaxy::error::Log::get().set_testing(false)

// clang-format on

namespace galaxy
{
	namespace error
	{
		///
		/// Log logging class.
		/// Uses multithreading.
		///
		class Log final
		{
		public:
			///
			/// Copy constructor.
			///
			Log(const Log&) = delete;

			///
			/// Move constructor.
			///
			Log(Log&&) = delete;

			///
			/// Copy assignment operator.
			///
			Log& operator=(const Log&) = delete;

			///
			/// Move assignment operator.
			///
			Log& operator=(Log&&) = delete;

			///
			/// Destructor.
			///
			~Log() = default;

			///
			/// Retrieve log instance.
			///
			/// \return Returns static reference to Log class.
			///
			static Log& get();

			///
			/// Initialize logging and set up destination file.
			///
			/// \param log_file File to write all log messages to.
			///
			void start(std::string_view log_file);

			///
			/// Manual control over closing streams.
			///
			void finish();

			///
			/// Log a message.
			///
			/// \param level Log error level.
			/// \param message Message to log.
			///
			template<error::type LogLevel, typename... MsgInputs>
			void log(const LogLevel, std::string_view message, const MsgInputs&... args /*std::source_location goes here*/);

			///
			/// Set testing mode.
			///
			/// \param is_testing Will not log if true.
			///
			void set_testing(const bool is_testing);

			///
			/// \brief	Set a minimum log level.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			/// \param level Level to set as the minimum level to log at.
			///
			template<error::type LogLevel>
			void set_min_level(const LogLevel);

		private:
			///
			/// Constructor.
			///
			Log();

			///
			/// Convert log message level to a string.
			///
			/// \param level Level to convert.
			///
			/// \return std::string, in caps.
			///
			template<error::type LogLevel>
			[[nodiscard]] std::string process_level();

			///
			/// Colourizes the terminal text based on the log message level.
			///
			/// \param level Level to use when selecting colour.
			///
			/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
			///
			template<error::type LogLevel>
			[[nodiscard]] std::string process_colour();

			///
			/// File stream to write to.
			///
			std::ofstream m_file_stream;

			///
			/// Minimum level of messages required to be logged.
			///
			Level m_min_level;

			///
			/// Protection mutex.
			///
			std::mutex m_mutex;

			///
			/// Current thread message.
			///
			std::string m_message;

			///
			/// Thread all logging takes place on.
			///
			std::jthread m_thread;

			///
			/// Controls thread loop.
			///
			std::atomic_bool m_running;

			///
			/// Wont log if testing mode is enabled.
			///
			bool m_testing_mode;
		};

		template<error::type LogLevel, typename... MsgInputs>
		inline void Log::log(const LogLevel, std::string_view message, const MsgInputs&... args)
		{
			if (!m_testing_mode)
			{
				// Check to make sure level should be logged.
				if (LogLevel::value() >= m_min_level)
				{
					std::lock_guard<std::mutex> lock {m_mutex};

					if (m_message.empty())
					{
						auto formatted_msg = fmt::format(message, args...);
						m_message          = fmt::format("{0}[{1}] - {2} - {3}\n", Log::get().process_colour<LogLevel>(), Log::get().process_level<LogLevel>(), date::format("%d-%m-%Y-[%H:%M]", date::make_zoned(date::current_zone(), std::chrono::system_clock::now())), formatted_msg);

						if constexpr (LogLevel::value() == Level::FATAL)
						{
							throw std::runtime_error {m_message};
						}
					}
				}
			}
		}

		template<error::type LogLevel>
		inline void Log::set_min_level(const LogLevel)
		{
			m_min_level = LogLevel::value();
		}

		template<error::type LogLevel>
		inline std::string Log::process_level()
		{
			if constexpr (LogLevel::value() == Level::INFO)
			{
				return "INFO";
			}
			else if constexpr (LogLevel::value() == Level::DEBUG)
			{
				return "DEBUG";
			}
			else if constexpr (LogLevel::value() == Level::WARNING)
			{
				return "WARNING";
			}
			else if constexpr (LogLevel::value() == Level::ERROR)
			{
				return "ERROR";
			}
			else if constexpr (LogLevel::value() == Level::FATAL)
			{
				return "FATAL";
			}
		}

		template<error::type LogLevel>
		inline std::string Log::process_colour()
		{
			if constexpr (LogLevel::value() == Level::INFO)
			{
				return "\x1B[37m";
			}
			else if constexpr (LogLevel::value() == Level::DEBUG)
			{
				return "\x1B[37m";
			}
			else if constexpr (LogLevel::value() == Level::WARNING)
			{
				return "\x1B[33m";
			}
			else if constexpr (LogLevel::value() == Level::ERROR)
			{
				return "\x1B[31m";
			}
			else if constexpr (LogLevel::value() == Level::FATAL)
			{
				return "\x1B[31m";
			}
		}
	} // namespace error
} // namespace galaxy

#endif