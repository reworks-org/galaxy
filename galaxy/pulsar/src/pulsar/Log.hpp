///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include <fstream>
#include <functional>
#include <future>
#include <mutex>

#include <date/tz.h>
#include <fmt/format.h>

#include "detail/Unix.hpp"
#include "detail/Windows.hpp"

///
/// Log macro for start().
///
/// \param log_file Log file path.
///
#define PL_LOG_START(log_file) pl::Log::get().start(log_file)

///
/// Log macro for start().
///
#define PL_LOG_FINISH pl::Log::get().finish()

///
/// Static interface handle.
///
#define PL_LOG_GET pl::Log::get()

///
/// INFO log level macro shortcut.
///
#define PL_INFO pl::Log::Level::INFO

///
/// DEBUG log level macro shortcut.
///
#define PL_DEBUG pl::Log::Level::DEBUG

///
/// WARNING log level macro shortcut.
///
#define PL_WARNING pl::Log::Level::WARNING

///
/// ERROR log level macro shortcut.
///
#define PL_ERROR pl::Log::Level::ERROR

///
/// FATAL log level macro shortcut.
///
#define PL_FATAL pl::Log::Level::FATAL

///
/// Macro shortcut with variadic arguments.
///
/// \param level Log error level.
/// \param message Message to log.
///
#define PL_LOG(...) pl::Log::get().log(__VA_ARGS__)

///
/// Enable testing mode.
///
#define PL_ENABLE_TESTING_MODE pl::Log::get().set_testing(true)

///
/// Disable testing mode.
///
#define PL_DISABLE_TESTING_MODE pl::Log::get().set_testing(false)

///
/// Core namespace.
///
namespace pl
{
	///
	/// Log logging class.
	/// Uses multithreading.
	///
	class Log final
	{
	public:
		///
		/// Enum defining the different reporting levels of a log message.
		///
		enum class Level : int
		{
			INFO    = 0,
			DEBUG   = 1,
			WARNING = 2,
			ERROR   = 3,
			FATAL   = 4
		};

		///
		/// Destructor.
		///
		~Log() noexcept;

		///
		/// Retrieve log instance.
		///
		/// \return Returns static reference to Log class.
		///
		static Log& get() noexcept;

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
		template<typename... msg_values>
		void log(const Log::Level level, const std::string& message, const msg_values&... args /*std::source_location goes here*/);

		///
		/// Set testing mode.
		///
		/// \param is_testing Will not log if true.
		///
		void set_testing(const bool is_testing) noexcept;

		///
		/// \brief	Set a minimum log level.
		///
		/// In order to only print and log levels greater than or equal to the current log message level.
		///
		/// \param level Level to set as the minimum level to log at.
		///
		void set_min_level(Log::Level level) noexcept;

		///
		/// Returns minimum logging message level that is required to log a message.
		///
		/// \return Log::Level enum.
		///
		[[nodiscard]] Log::Level get_min_level() noexcept;

	private:
		///
		/// Constructor.
		///
		Log() noexcept;

		///
		/// Delete Copy construct in order to preserve singleton.
		///
		Log(Log const&) = delete;

		///
		/// Delete Move construct in order to preserve singleton.
		///
		Log(Log&&) = delete;

		///
		/// Delete Copy assign in order to preserve singleton.
		///
		Log& operator=(Log const&) = delete;

		///
		/// Delete Move assign in order to preserve singleton.
		///
		Log& operator=(Log&&) = delete;

		///
		/// Convert log message level to a string.
		///
		/// \param level Level to convert.
		///
		/// \return std::string, in caps.
		///
		[[nodiscard]] std::string process_level(const Log::Level level);

		///
		/// Colourizes the terminal text based on the log message level.
		///
		/// \param level Level to use when selecting colour.
		///
		/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
		///
		[[nodiscard]] std::string process_colour(Log::Level level);

		///
		/// Filters a log stream message based on message level to determine if it must be logged.
		///
		/// \param level Level of current message to determine if it must be logged.
		///
		/// \return True if can log.
		///
		[[nodiscard]] bool filter_level(Log::Level level) noexcept;

		///
		/// File stream to write to.
		///
		std::ofstream m_file_stream;

		///
		/// Minimum level of messages required to be logged.
		///
		Log::Level m_min_level;

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
		std::future<void> m_thread;

		///
		/// Controls thread loop.
		///
		std::atomic_bool m_running;

		///
		/// Wont log if testing mode is enabled.
		///
		bool m_testing_mode;
	};

	template<typename... msg_values>
	inline void Log::log(const Log::Level level, const std::string& message, const msg_values&... args)
	{
		if (!m_testing_mode)
		{
			// Check to make sure level should be logged.
			if (Log::get().filter_level(level))
			{
				std::lock_guard<std::mutex> lock {m_mutex};

				if (m_message.empty())
				{
					auto formatted_msg = fmt::format(message, args...);

					// Create log message string.
					m_message = fmt::format("{0}[{1}] - {2} - {3}\n", Log::get().process_colour(level), Log::get().process_level(level), date::format("%m/%d/%Y %H:%M\n", date::make_zoned(date::current_zone(), std::chrono::system_clock::now())), formatted_msg);

					if (level == PL_FATAL)
					{
						throw std::runtime_error {m_message};
					}
				}
			}
		}
	}
} // namespace pl

#endif