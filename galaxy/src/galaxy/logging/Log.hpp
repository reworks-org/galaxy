///
/// Log.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOG_HPP_
#define GALAXY_LOGGING_LOG_HPP_

#include <filesystem>
#include <source_location>
#include <stacktrace>

#include <entt/locator/locator.hpp>
#include <magic_enum/magic_enum.hpp>

#include "galaxy/logging/LogLevel.hpp"
#include "galaxy/logging/Sink.hpp"
#include "galaxy/utils/StringUtils.hpp"

#define GALAXY_INFO                     galaxy::logging::LogLevel::_INFO_
#define GALAXY_DEBUG                    galaxy::logging::LogLevel::_DEBUG_
#define GALAXY_WARNING                  galaxy::logging::LogLevel::_WARNING_
#define GALAXY_ERROR                    galaxy::logging::LogLevel::_ERROR_
#define GALAXY_FATAL                    galaxy::logging::LogLevel::_FATAL_
#define GALAXY_LOG_SET_MIN_LEVEL(level) entt::locator<galaxy::logging::Log>::value().set_min_level<level>()
#define GALAXY_ADD_SINK(sink, ...)      entt::locator<galaxy::logging::Log>::value().add_sink<sink>(__VA_ARGS__)
#define GALAXY_LOG(level, msg, ...)     entt::locator<galaxy::logging::Log>::value().log<level>(std::source_location::current(), msg __VA_OPT__(, ) __VA_ARGS__)

namespace galaxy
{
	namespace logging
	{
		///
		/// \brief Sink based logging system.
		///
		/// Use macros to access.
		///
		class Log final
		{
		public:
			///
			/// Constructor.
			///
			Log() noexcept;

			///
			/// Destructor.
			///
			~Log() noexcept;

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
			[[maybe_unused]]
			SinkTo& add_sink(Args&&... args);

			///
			/// \brief Set a minimum log level.
			///
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			/// \tparam level Must be a LogLevel enum value.
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

		private:
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
			SinkTo* ptr = dynamic_cast<SinkTo*>(m_sinks.back().get());
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
				auto trace = std::stacktrace::current();

				// clang-format off
				LogMessage lm
				{
					.time    = std::format("{0:%r}", std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time()),
					.file     = std::filesystem::path(loc.file_name()).filename().string(),
					.line    = std::to_string(loc.line()),
					.message = std::vformat(message, std::make_format_args(args...)),
					.trace = std::to_string(trace)
				};
				// clang-format on

				lm.level = magic_enum::enum_name<LogLevel>(level);
				strutils::replace_all(lm.level, "_", "");

				if constexpr (level == LogLevel::_INFO_ || level == LogLevel::_DEBUG_)
				{
					lm.colour = "\x1b[1m\x1b[33m";
				}
				else if constexpr (level == LogLevel::_WARNING_)
				{
					lm.colour = "\x1b[1m\x1b[31m";
				}
				else if constexpr (level == LogLevel::_ERROR_ || level == LogLevel::_FATAL_)
				{
					lm.colour = "\x1b[41m\x1b[37m";
				}

				for (const auto& sink : m_sinks)
				{
					sink->sink_message(lm);
				}

				if constexpr (level == LogLevel::_FATAL_)
				{
					throw std::runtime_error(std::to_string(trace));
				}
			}
		}
	} // namespace logging
} // namespace galaxy

#endif
