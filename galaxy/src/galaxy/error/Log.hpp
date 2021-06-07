///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOG_HPP_
#define GALAXY_ERROR_LOG_HPP_

#include <chrono>
#include <filesystem>
#include <fstream>
#include <source_location>

#if defined(_WIN32) || defined(_WIN64)
	#include <format>
#else
	#include <fmt/format.h>
	namespace std
	{
		constexpr auto format = [] (auto&& ...args)
		{
			return fmt::format(std::forward<decltype(args)>(args)...);
		};
	}
#endif

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
#define GALAXY_INFO galaxy::error::Level::INFO

///
/// DEBUG log level macro shortcut.
///
#define GALAXY_DEBUG galaxy::error::Level::DEBUG

///
/// WARNING log level macro shortcut.
///
#define GALAXY_WARNING galaxy::error::Level::WARNING

///
/// ERROR log level macro shortcut.
///
#define GALAXY_ERROR galaxy::error::Level::ERROR_

///
/// FATAL log level macro shortcut.
///
#define GALAXY_FATAL galaxy::error::Level::FATAL

///
/// Macro shortcut with variadic arguments.
///
/// \param level Log error level.
/// \param msg Error message.
/// \param ... Message and arguments to format and log.
///
#define GALAXY_LOG(level, msg, ...) galaxy::error::Log::handle().log<level>(std::source_location::current(), msg __VA_OPT__(,) __VA_ARGS__)

///
/// Capture log output into a custom stream.
///
/// \param ostream Custom stream to output to.
///
#define GALAXY_LOG_CAPTURE_CUSTOM(ostream) galaxy::error::Log::handle().change_stream(ostream)

// clang-format on

namespace galaxy
{
	namespace error
	{
		///
		/// Different levels of error messaging.
		///
		enum class Level : short
		{
			///
			/// Info Log Level.
			///
			INFO = 0,

			///
			/// Debug Log Level.
			///
			DEBUG = 1,

			///
			/// Warning Log Level.
			///
			WARNING = 2,

			///
			/// Error Log Level.
			///
			ERROR_ = 3,

			///
			/// Fatal Log Level.
			///
			FATAL = 4
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
			/// \param ostream Custom stream to output to.
			///
			void change_stream(std::ostream& ostream) noexcept;

			///
			/// Log a message.
			///
			/// \param message Message to log.
			/// \param args std::format supported arguments to be formatted into a string.
			///
			template<Level log_level, typename... MsgInputs>
			void log(const std::source_location& loc, std::string_view message, const MsgInputs&... args);

			///
			/// \brief	Set a minimum log level.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			template<Level log_level>
			void set_min_level() noexcept;

		private:
			///
			/// Constructor.
			///
			Log();

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
			Level m_min_level;

			///
			/// Flag to check if start() has been called.
			///
			bool m_started;
		};

		template<Level log_level, typename... MsgInputs>
		inline void Log::log(const std::source_location& loc, std::string_view message, const MsgInputs&... args)
		{
			if ((log_level >= m_min_level) && m_started)
			{
				static std::string colour {""};
				static std::string level {""};

				if constexpr (log_level == Level::INFO)
				{
					static const std::string l_info("INFO");
					static const std::string c_info("\x1B[37m");

					level  = l_info;
					colour = c_info;
				}
				else if constexpr (log_level == Level::DEBUG)
				{
					static const std::string l_debug {"DEBUG"};
					static const std::string c_debug {"\x1B[37m"};

					level  = l_debug;
					colour = c_debug;
				}
				else if constexpr (log_level == Level::WARNING)
				{
					static const std::string l_warn {"WARNING"};
					static const std::string c_warn {"\x1B[33m"};

					level  = l_warn;
					colour = c_warn;
				}
				else if constexpr (log_level == Level::ERROR_)
				{
					static const std::string l_err {"ERROR"};
					static const std::string c_err {"\x1B[31m"};

					level  = l_err;
					colour = c_err;
				}
				else if constexpr (log_level == Level::FATAL)
				{
					static const std::string l_fatal {"FATAL"};
					static const std::string c_fatal {"\x1B[31m"};

					level  = l_fatal;
					colour = c_fatal;
				}

				const auto now       = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
				const auto final_str = std::format("{0}[{1}] - [{2}] - [{3}] - \"{4}\"\n", colour, level, std::format("{0:%T}", now), std::format("File: {0}, Func: {1}, Line: {2}", std::filesystem::path(loc.file_name()).filename().string(), loc.function_name(), loc.line()), std::format(message, args...));

				*m_stream << final_str;
				m_file_stream << final_str;

				if constexpr (log_level == Level::FATAL)
				{
					throw std::runtime_error {final_str};
				}
			}
		}

		template<Level log_level>
		inline void Log::set_min_level() noexcept
		{
			m_min_level = log_level;
		}
	} // namespace error
} // namespace galaxy

#endif