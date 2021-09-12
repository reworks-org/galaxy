///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOG_HPP_
#define GALAXY_ERROR_LOG_HPP_

#include <filesystem>
#include <future>
#include <queue>
#include <source_location>

#include <magic_enum.hpp>

#include "galaxy/error/LogLevel.hpp"
#include "galaxy/error/Sink.hpp"

// clang-format off

///
/// INFO log level macro shortcut.
///
#define GALAXY_INFO galaxy::error::LogLevel::INFO

///
/// DEBUG log level macro shortcut.
///
#define GALAXY_DEBUG galaxy::error::LogLevel::DEBUG

///
/// WARNING log level macro shortcut.
///
#define GALAXY_WARNING galaxy::error::LogLevel::WARNING

///
/// ERROR log level macro shortcut.
///
#define GALAXY_ERROR galaxy::error::LogLevel::ERROR_

///
/// FATAL log level macro shortcut.
///
#define GALAXY_FATAL galaxy::error::LogLevel::FATAL

///
/// Start logging thread.
///
#define GALAXY_LOG_START galaxy::error::Log::handle().start

///
/// Cleanup static logging resources.
///
#define GALAXY_LOG_END galaxy::error::Log::handle().cleanup

///
/// Set the minimum level of logging.
///
#define GALAXY_LOG_SET_MIN_LEVEL(level) galaxy::error::Log::handle().set_min_level<level>()

///
/// Configure a logging sink.
///
#define GALAXY_ADD_SINK(sink, ...) galaxy::error::Log::handle().add_sink<sink>(__VA_ARGS__)

///
/// Macro shortcut with variadic arguments.
///
/// \param level Log error level.
/// \param msg Error message.
/// \param ... Message and arguments to format and log.
///
#define GALAXY_LOG(level, msg, ...) galaxy::error::Log::handle().log<level>(std::source_location::current(), msg __VA_OPT__(,) __VA_ARGS__)

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
			/// Start logging thread.
			///
			void start() noexcept;

			///
			/// Add a sink to log to.
			///
			/// \param args Constructor arguments for a sink. Can be blank.
			///
			/// \return A pointer to the newly created sink.
			///
			template<std::derived_from<Sink> SinkTo, typename... Args>
			[[maybe_unused]] SinkTo* add_sink(Args&&... args);

			///
			/// \brief Set a minimum log level.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			template<LogLevel level>
			void set_min_level() noexcept;

			///
			/// Log a message.
			///
			/// \param loc Source location argument.
			/// \param message Message to log.
			/// \param args std::format supported arguments to be formatted into a string.
			///
			template<LogLevel level, typename... MsgInputs>
			void log(const std::source_location& loc, std::string_view message, const MsgInputs&... args);

			///
			/// Cleanup any static resources.
			///
			void cleanup() noexcept;

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
			/// Minimum level of messages required to be logged.
			///
			LogLevel m_min_level;

			///
			/// Logging thread handle.
			///
			std::future<void> m_async;

			///
			/// List of sinks.
			///
			std::vector<std::unique_ptr<Sink>> m_sinks;

			///
			/// Message queue.
			///
			std::queue<std::string> m_messages;

			///
			/// Flag to keep/shutdown thread.
			///
			std::atomic_bool m_run_thread;

			///
			/// Protects queue from data races.
			///
			std::mutex m_log_lock;
		};

		template<std::derived_from<Sink> SinkTo, typename... Args>
		inline SinkTo* Log::add_sink(Args&&... args)
		{
			m_sinks.push_back(std::make_unique<SinkTo>(std::forward<Args>(args)...));
			return static_cast<SinkTo*>(m_sinks.back().get());
		}

		template<LogLevel level>
		inline void Log::set_min_level() noexcept
		{
			m_min_level = level;
		}

		template<LogLevel level, typename... MsgInputs>
		inline void Log::log(const std::source_location& loc, std::string_view message, const MsgInputs&... args)
		{
			if (level >= m_min_level)
			{
				constexpr auto level_str = magic_enum::enum_name(level);

				constexpr auto low_error_colour  = "\x1B[37m";
				constexpr auto mid_error_colour  = "\x1B[33m";
				constexpr auto high_error_colour = "\x1B[31m";

				static std::string colour;

				if constexpr (level == LogLevel::INFO || level == LogLevel::DEBUG)
				{
					colour = low_error_colour;
				}
				else if constexpr (level == LogLevel::WARNING)
				{
					colour = mid_error_colour;
				}
				else if constexpr (level == LogLevel::ERROR_ || level == LogLevel::FATAL)
				{
					colour = high_error_colour;
				}

				const auto now       = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();
				const auto final_str = std::format(
					"{0}[{1}] - [{2}] - [{3}] - \"{4}\"\n",
					colour,
					level_str,
					std::format("{0:%T}", now),
					std::format("File: {0}, Func: {1}, Line: {2}", std::filesystem::path(loc.file_name()).filename().string(), loc.function_name(), loc.line()),
					std::format(message, args...));

				{
					std::lock_guard<std::mutex> lock {m_log_lock};
					m_messages.push(final_str);
				}

				if constexpr (level == LogLevel::FATAL)
				{
					throw std::runtime_error {final_str};
				}
			}
		}
	} // namespace error
} // namespace galaxy

#endif