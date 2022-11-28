///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOG_HPP_
#define GALAXY_ERROR_LOG_HPP_

#include <filesystem>
#include <source_location>

#include <magic_enum.hpp>

#include "galaxy/error/LogLevel.hpp"
#include "galaxy/error/Sink.hpp"

// clang-format off
#ifdef _DEBUG
#define GALAXY_INFO                     galaxy::error::LogLevel::INFO
#define GALAXY_DEBUG                    galaxy::error::LogLevel::DEBUG
#define GALAXY_WARNING                  galaxy::error::LogLevel::WARNING
#define GALAXY_ERROR                    galaxy::error::LogLevel::ERROR
#define GALAXY_FATAL                    galaxy::error::LogLevel::FATAL
#define GALAXY_LOG_START                galaxy::error::Log::handle().start
#define GALAXY_LOG_FINISH               galaxy::error::Log::handle().finish
#define GALAXY_LOG_SET_MIN_LEVEL(level) galaxy::error::Log::handle().set_min_level<level>()
#define GALAXY_ADD_SINK(sink, ...)      galaxy::error::Log::handle().add_sink<sink>(__VA_ARGS__)
#define GALAXY_LOG(level, msg, ...)     galaxy::error::Log::handle().log<level>(std::source_location::current(), msg __VA_OPT__(, ) __VA_ARGS__)
#else
#define GALAXY_INFO                     {}
#define GALAXY_DEBUG                    {}
#define GALAXY_WARNING                  {}
#define GALAXY_ERROR                    {}
#define GALAXY_FATAL                    {}
#define GALAXY_LOG_START()              {}
#define GALAXY_LOG_FINISH()             {}
#define GALAXY_LOG_SET_MIN_LEVEL(level) {}
#define GALAXY_ADD_SINK(sink, ...)      {}
#define GALAXY_LOG(level, msg, ...)     {}
#endif
// clang-format on

namespace galaxy
{
	namespace error
	{
		///
		/// \brief Log singleton.
		///
		/// Use macros to access.
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
			/// \tparam SinkTo The derived type of the sink.
			/// \tparam Args Variadic arguments for sink constructor.
			///
			/// \param args Constructor arguments for a sink. Can be blank.
			///
			/// \return A pointer to the newly created sink.
			///
			template<std::derived_from<Sink> SinkTo, typename... Args>
			[[maybe_unused]] SinkTo& add_sink(Args&&... args);

			///
			/// \brief Set a minimum log level.
			///
			/// \tparam level Must be a LogLevel enum value.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			template<LogLevel level>
			void set_min_level() noexcept;

			///
			/// Log a message.
			///
			/// \tparam level Must be a LogLevel enum value.
			/// \tparam MsgInputs Variadic arguments for std::format string.
			///
			/// \param loc Source location argument.
			/// \param message Message template to log.
			/// \param args std::format supported arguments to be formatted into a string.
			///
			template<LogLevel level, typename... MsgInputs>
			void log(const std::source_location& loc, std::string_view message, const MsgInputs&... args);

			///
			/// Cleanup any static resources.
			///
			void finish() noexcept;

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
			/// Minimum level for a message to be logged.
			///
			LogLevel m_min_level;

			///
			/// List of sinks.
			///
			std::vector<std::unique_ptr<Sink>> m_sinks;
		};

		template<std::derived_from<Sink> SinkTo, typename... Args>
		inline SinkTo& Log::add_sink(Args&&... args)
		{
			m_sinks.push_back(std::make_unique<SinkTo>(std::forward<Args>(args)...));
			SinkTo* ptr = static_cast<SinkTo*>(m_sinks.back().get());
			return *ptr;
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
				constexpr const auto level_str = magic_enum::enum_name(level);

				// Prevents needless memory reallocation.
				// One copy per thread.
				thread_local std::string final_str;
				final_str.clear();

				const auto now = std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time();

				if constexpr (level == LogLevel::INFO || level == LogLevel::DEBUG)
				{
					final_str = std::format("{0}[{1}] {2} | {3} \"{4}\"\n",
						"\x1B[37m",
						std::format("{0:%T}", now),
						level_str,
						std::format("File: {0}, Func: {1}, Line: {2}, Message: ",
							std::filesystem::path(loc.file_name()).filename().string(),
							loc.function_name(),
							loc.line()),
						std::vformat(message, std::make_format_args(args...)));
				}
				else if constexpr (level == LogLevel::WARNING)
				{
					final_str = std::format("{0}[{1}] {2} | {3} \"{4}\"\n",
						"\x1B[33m",
						std::format("{0:%T}", now),
						level_str,
						std::format("File: {0}, Func: {1}, Line: {2}, Message: ",
							std::filesystem::path(loc.file_name()).filename().string(),
							loc.function_name(),
							loc.line()),
						std::vformat(message, std::make_format_args(args...)));
				}
				else if constexpr (level == LogLevel::ERROR || level == LogLevel::FATAL)
				{
					final_str = std::format("{0}[{1}] {2} | {3} \"{4}\"\n",
						"\x1B[31m",
						std::format("{0:%T}", now),
						level_str,
						std::format("File: {0}, Func: {1}, Line: {2}, Message: ",
							std::filesystem::path(loc.file_name()).filename().string(),
							loc.function_name(),
							loc.line()),
						std::vformat(message, std::make_format_args(args...)));
				}

				for (const auto& sink : m_sinks)
				{
					sink->sink_message(final_str);
				}

				if constexpr (level == LogLevel::FATAL)
				{
					throw std::runtime_error(std::vformat(message, std::make_format_args(args...)));
				}
			}
		}
	} // namespace error
} // namespace galaxy

#endif