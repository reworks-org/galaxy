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

#include <magic_enum/magic_enum.hpp>
#include <readerwriterqueue.h>

#include "galaxy/error/LogLevel.hpp"
#include "galaxy/error/Sink.hpp"
#include "galaxy/meta/Memory.hpp"

#define GALAXY_INFO                     galaxy::error::LogLevel::INFO
#define GALAXY_DEBUG                    galaxy::error::LogLevel::DEBUG
#define GALAXY_WARNING                  galaxy::error::LogLevel::WARNING
#define GALAXY_ERROR                    galaxy::error::LogLevel::ERROR
#define GALAXY_FATAL                    galaxy::error::LogLevel::FATAL
#define GALAXY_LOG_INIT                 galaxy::error::Log::handle().init
#define GALAXY_LOG_FINISH               galaxy::error::Log::handle().finish
#define GALAXY_LOG_SET_MIN_LEVEL(level) galaxy::error::Log::handle().set_min_level<level>()
#define GALAXY_ADD_SINK(sink, ...)      galaxy::error::Log::handle().add_sink<sink>(__VA_ARGS__)
#define GALAXY_LOG(level, msg, ...)     galaxy::error::Log::handle().log<level>(std::source_location::current(), msg __VA_OPT__(, ) __VA_ARGS__)

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
			~Log();

			///
			/// Retrieve log instance.
			///
			/// \return Returns static reference to Log class.
			///
			[[nodiscard]] static Log& handle();

			///
			/// Initialize logging resources.
			///
			void init();

			///
			/// Cleanup any static resources and prepare to close threads.
			///
			void finish();

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
			/// In order to only print and log levels greater than or equal to the current log message level.
			///
			/// \tparam level Must be a LogLevel enum value.
			///
			template<LogLevel level>
			void set_min_level();

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
			/// Keeps logging thread alive.
			///
			std::atomic_bool m_active;

			///
			/// Lock-free queue for single producer/consumer.
			///
			moodycamel::ReaderWriterQueue<LogMessage> m_queue;

			///
			/// Minimum level for a message to be logged.
			///
			LogLevel m_min_level;

			///
			/// List of sinks.
			///
			meta::vector<std::unique_ptr<Sink>> m_sinks;
		};

		template<std::derived_from<Sink> SinkTo, typename... Args>
		inline SinkTo& Log::add_sink(Args&&... args)
		{
			m_sinks.push_back(std::make_unique<SinkTo>(std::forward<Args>(args)...));
			SinkTo* ptr = dynamic_cast<SinkTo*>(m_sinks.back().get());
			return *ptr;
		}

		template<LogLevel level>
		inline void Log::set_min_level()
		{
			m_min_level = level;
		}

		template<LogLevel level, typename... MsgInputs>
		inline void Log::log(const std::source_location& loc, std::string_view message, const MsgInputs&... args)
		{
			if (level >= m_min_level)
			{
				// clang-format off
				LogMessage lm
				{
					.time    = std::format("{0:%r}", std::chrono::zoned_time {std::chrono::current_zone(), std::chrono::system_clock::now()}.get_local_time()),
					.file     = loc.file_name(),
					.line    = std::to_string(loc.line()),
					.message = std::vformat(message, std::make_format_args(args...))
				};
				// clang-format on

				lm.level = magic_enum::enum_name<LogLevel>(level);

				if constexpr (level == LogLevel::INFO || level == LogLevel::DEBUG)
				{
					lm.colour = "\x1B[37m";
				}
				else if constexpr (level == LogLevel::WARNING)
				{
					lm.colour = "\x1B[33m";
				}
				else if constexpr (level == LogLevel::ERROR || level == LogLevel::FATAL)
				{
					lm.colour = "\x1B[31m";
				}

				m_queue.enqueue(lm);

				if constexpr (level == LogLevel::FATAL)
				{
					throw std::runtime_error(lm.message);
				}
			}
		}
	} // namespace error
} // namespace galaxy

#endif
