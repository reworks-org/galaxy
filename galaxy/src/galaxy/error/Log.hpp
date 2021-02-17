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
#include <mutex>
#include <sstream>
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
/// Capture log output into a custom stream.
///
/// \param stream Custom stream to output to.
///
#define GALAXY_LOG_CAPTURE_CUSTOM(ostream) galaxy::error::Log::handle().change_stream(ostream)

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
			/// Destructor.
			///
			~Log() noexcept = default;

			///
			/// Retrieve log instance.
			///
			/// \return Returns static reference to Log class.
			///
			[[nodiscard]] static Log& handle() noexcept;

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
			/// Capture log output into a custom stream.
			///
			/// \param stream Custom stream to output to.
			///
			void change_stream(std::ostream& stream);

			///
			/// Log a message.
			///
			/// \param message Message to log.
			/// \param args std::format supported arguments to be formatted into a string.
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

		private:
			///
			/// Output stream to write to.
			///
			std::ostream* m_stream;

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

		///
		/// Convert log message level to a string.
		///
		/// \param level Level to convert.
		///
		/// \return std::string, in caps.
		///
		template<loglevel_type LogLevel>
		[[nodiscard]] inline constexpr const char* process_level() noexcept
		{
			if constexpr (LogLevel::value == level::Info::value)
			{
				constexpr const char* out = "INFO";
				return out;
			}
			else if constexpr (LogLevel::value == level::Debug::value)
			{
				constexpr const char* out = "DEBUG";
				return out;
			}
			else if constexpr (LogLevel::value == level::Warning::value)
			{
				constexpr const char* out = "WARNING";
				return out;
			}
			else if constexpr (LogLevel::value == level::Error::value)
			{
				constexpr const char* out = "ERROR";
				return out;
			}
			else if constexpr (LogLevel::value == level::Fatal::value)
			{
				constexpr const char* out = "FATAL";
				return out;
			}
		}

		///
		/// Colourizes the terminal text based on the log message level.
		///
		/// \param level Level to use when selecting colour.
		///
		/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
		///
		template<loglevel_type LogLevel>
		[[nodiscard]] inline constexpr const char* process_colour() noexcept
		{
			if constexpr (LogLevel::value == level::Info::value)
			{
				constexpr const char* out = "\x1B[37m";
				return out;
			}
			else if constexpr (LogLevel::value == level::Debug::value)
			{
				constexpr const char* out = "\x1B[37m";
				return out;
			}
			else if constexpr (LogLevel::value == level::Warning::value)
			{
				constexpr const char* out = "\x1B[33m";
				return out;
			}
			else if constexpr (LogLevel::value == level::Error::value)
			{
				constexpr const char* out = "\x1B[31m";
				return out;
			}
			else if constexpr (LogLevel::value == level::Fatal::value)
			{
				constexpr const char* out = "\x1B[31m";
				return out;
			}
		}

		template<loglevel_type LogLevel, typename... MsgInputs>
		inline void Log::log(std::string_view message, const MsgInputs&... args)
		{
			if (LogLevel::value >= m_min_level)
			{
				std::lock_guard<std::mutex> lock {m_msg_mutex};

				constexpr const char* colour = process_colour<LogLevel>();
				constexpr const char* level  = process_level<LogLevel>();

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

		template<loglevel_type LogLevel>
		inline void Log::set_min_level() noexcept
		{
			m_min_level = LogLevel::value;
		}
	} // namespace error
} // namespace galaxy

#endif