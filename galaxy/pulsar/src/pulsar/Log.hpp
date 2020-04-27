///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include <mutex>
#include <fstream>
#include <functional>

#include "detail/Unix.hpp"
#include "detail/Windows.hpp"

///
/// Macro shortcut.
///
/// \param level Log error level.
/// \param message Message to log.
///
#define PL_LOG(level, message) pulsar::Log::get().log(level, message)

///
/// INFO log level macro shortcut.
///
#define PL_INFO pulsar::Log::Level::INFO

///
/// DEBUG log level macro shortcut.
///
#define PL_DEBUG pulsar::Log::Level::DEBUG

///
/// WARNING log level macro shortcut.
///
#define PL_WARNING pulsar::Log::Level::WARNING

///
/// ERROR log level macro shortcut.
///
#define PL_ERROR pulsar::Log::Level::ERROR_

///
/// FATAL log level macro shortcut.
///
#define PL_FATAL pulsar::Log::Level::FATAL

///
/// Core namespace.
///
namespace pulsar
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
		~Log() noexcept;

		///
		/// Enum defining the different reporting levels of a log message.
		///
		enum class Level : int
		{
			INFO = 0,
			DEBUG = 1,
			WARNING = 2,
			ERROR_ = 3,
			FATAL = 4
		};

		///
		/// Retrieve log instance.
		///
		/// \return Returns static reference to Log class.
		///
		static Log& get();

		///
		/// Initialize logging and set up destination file.
		///
		/// \param logTo File to write all log messages to.
		///
		void init(const std::string& logTo);

		///
		/// Manual control over deinitialization.
		///
		void deinit() noexcept;

		///
		/// Log a message.
		///
		/// \param level Log error level.
		/// \param message Message to log.
		///
		void log(const pulsar::Log::Level level, const std::string& message);

		///
		/// Convert log message level to a string.
		///
		/// \param level Level to convert.
		///
		/// \return std::string, in caps.
		///
		std::string processLevel(const pulsar::Log::Level level);

		///
		/// Colourizes the terminal text based on the log message level.
		///
		/// \param level Level to use when selecting colour.
		///
		/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
		///
		std::string processColour(pulsar::Log::Level level);
		
		///
		/// Filters a log stream message based on message level to determine if it must be logged.
		///
		/// \param level Level of current message to determine if it must be logged.
		///
		/// \return True if can log.
		///
		bool filterLevel(pulsar::Log::Level level);
		
		///
		/// \brief	Set a minimum log level.
		///
		/// In order to only print and log levels greater than or equal to the current log message level.
		///
		/// \param level Level to set as the minimum level to log at.
		///
		void setMinimumLevel(pulsar::Log::Level level);
		
		///
		/// Returns minimum logging message level that is required to log a message.
		///
		/// \return pulsar::Log::Level enum.
		///
		pulsar::Log::Level getMinimumLevel();
		
		///
		/// Gets current date and time in a string format.
		///
		/// \return Returns date/time as a std::string.
		///
		std::string getDateTime();

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

	private:
		///
		/// Mutex to protect from multiple accesses.
		///
		std::mutex m_lock;

		///
		/// Stored futures.
		///
		std::vector<std::future<void>> m_futures;

		///
		/// File stream to write to.
		///
		std::ofstream m_fileStream;

		///
		/// Minimum level of messages required to be logged.
		///
		pulsar::Log::Level m_minimumLevel;

		///
		/// Thread callback function.
		///
		std::function<void(const pulsar::Log::Level, const std::string&)> m_callback;
	};
}

#endif