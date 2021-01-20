///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOG_HPP_
#define GALAXY_ERROR_LOG_HPP_

#include <fstream>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <thread>

#include <fmt/format.h>

#include "galaxy/error/LogLevel.hpp"

// clang-format off

///
/// Log macro for start().
///
/// \param log_file Log file path.
///
#define GALAXY_LOG_START(log_file) galaxy::error::Log::handle().start(log_file)

///
/// Log macro for start().
///
#define GALAXY_LOG_FINISH galaxy::error::Log::handle().finish()

///
/// Set the minimum level of logging.
///
#define GALAXY_LOG_SET_MIN_LEVEL(level) galaxy::error::Log::handle().set_min_level<level>()

///
/// INFO log level macro shortcut.
///
#define GALAXY_INFO galaxy::error::level::Info

///
/// DEBUG log level macro shortcut.
///
#define GALAXY_DEBUG galaxy::error::level::Debug

///
/// WARNING log level macro shortcut.
///
#define GALAXY_WARNING galaxy::error::level::Warning

///
/// ERROR log level macro shortcut.
///
#define GALAXY_ERROR galaxy::error::level::Error

///
/// FATAL log level macro shortcut.
///
#define GALAXY_FATAL galaxy::error::level::Fatal

///
/// Macro shortcut with variadic arguments.
///
/// \param level Log error level.
/// \param ... Message and arguments to format and log.
///
#define GALAXY_LOG(level, ...) galaxy::error::Log::handle().log<level>(__VA_ARGS__)

///
/// Enable testing mode.
///
#define GALAXY_ENABLE_TESTING_MODE galaxy::error::TestingMode::value = galaxy::error::TestingMode::true_val

///
/// Disable testing mode.
///
#define GALAXY_DISABLE_TESTING_MODE galaxy::error::TestingMode::value = galaxy::error::TestingMode::false_val

// clang-format on

namespace galaxy
{
	namespace error
	{
		///
		/// \brief Testing mode.
		///
		/// constexpr utilities.
		///
		struct TestingMode
		{
			inline static constexpr auto true_val  = true;
			inline static constexpr auto false_val = false;

			inline static constexpr bool value = false;
		};

		///
		/// Log logging class.
		/// Uses multithreading.
		///
		class Log final
		{
		public:
			///
			/// Destructor.
			///
			~Log() noexcept = default;

			///
			/// Retrieve log instance.
			///
			/// \return Returns static reference to Log class.
			///
			[[no_discard]] static Log& handle() noexcept;

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
			template<loglevel_type LogLevel, typename... MsgInputs>
			void log(std::string_view message, const MsgInputs&... args /*std::source_location goes here*/);

			///
			/// \brief	Set a minimum log level.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			template<loglevel_type LogLevel>
			void set_min_level() noexcept;

		private:
			///
			/// Constructor.
			///
			Log() noexcept;

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
			/// Convert log message level to a string.
			///
			/// \param level Level to convert.
			///
			/// \return std::string, in caps.
			///
			template<loglevel_type LogLevel>
			[[nodiscard]] constexpr const char* process_level() noexcept;

			///
			/// Colourizes the terminal text based on the log message level.
			///
			/// \param level Level to use when selecting colour.
			///
			/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
			///
			template<loglevel_type LogLevel>
			[[nodiscard]] constexpr const char* process_colour() noexcept;

		private:
			///
			/// File stream to write to.
			///
			std::ofstream m_file_stream;

			///
			/// Minimum level of messages required to be logged.
			///
			int m_min_level;

			///
			/// Thread all logging takes place on.
			///
			std::jthread m_thread;

			///
			/// Message mutex.
			///
			std::mutex m_msg_mutex;

			///
			/// Cross-Thread message
			///
			std::string m_message;
		};

		template<loglevel_type LogLevel, typename... MsgInputs>
		inline void Log::log(std::string_view message, const MsgInputs&... args)
		{
			if constexpr (!TestingMode::value)
			{
				if (LogLevel::value >= m_min_level)
				{
					std::lock_guard<std::mutex> lock {m_msg_mutex};

					const constexpr auto colour = process_colour<LogLevel>();
					const constexpr auto level  = process_level<LogLevel>();

					const auto fmt_msg  = fmt::format(message, args...);
					const auto time_obj = std::time(nullptr);

					std::stringstream sstream;
					sstream << std::put_time(std::localtime(&time_obj), "%d-%m-%Y-[%H:%M]");

					m_message = fmt::format("{0}[{1}] - {2} - {3}\n", colour, level, sstream.str(), fmt_msg);
					if constexpr (LogLevel::value == level::Fatal::value)
					{
						throw std::runtime_error {m_message};
					}
				}
			}
		}

		template<loglevel_type LogLevel>
		inline void Log::set_min_level() noexcept
		{
			m_min_level = LogLevel::value;
		}

		template<loglevel_type LogLevel>
		inline constexpr const char* Log::process_level() noexcept
		{
			if constexpr (LogLevel::value == level::Info::value)
			{
				return "INFO";
			}
			else if constexpr (LogLevel::value == level::Debug::value)
			{
				return "DEBUG";
			}
			else if constexpr (LogLevel::value == level::Warning::value)
			{
				return "WARNING";
			}
			else if constexpr (LogLevel::value == level::Error::value)
			{
				return "ERROR";
			}
			else if constexpr (LogLevel::value == level::Fatal::value)
			{
				return "FATAL";
			}
		}

		template<loglevel_type LogLevel>
		inline constexpr const char* Log::process_colour() noexcept
		{
			if constexpr (LogLevel::value == level::Info::value)
			{
				return "\x1B[37m";
			}
			else if constexpr (LogLevel::value == level::Debug::value)
			{
				return "\x1B[37m";
			}
			else if constexpr (LogLevel::value == level::Warning::value)
			{
				return "\x1B[33m";
			}
			else if constexpr (LogLevel::value == level::Error::value)
			{
				return "\x1B[31m";
			}
			else if constexpr (LogLevel::value == level::Fatal::value)
			{
				return "\x1B[31m";
			}
		}
	} // namespace error
} // namespace galaxy

#endif