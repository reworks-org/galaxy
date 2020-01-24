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
/// Singleton shortcut.
///
#define PL_LOG_I pl::Log::i()

///
/// Macro shortcut.
///
/// \param level Log error level.
/// \param message Message to log.
///
#define PL_LOG(level, message) pl::Log::i().log(level, message)

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
			ERROR = 3,
			FATAL = 4
		};

		///
		/// Retrieve log instance.
		///
		/// \return Returns static reference to Log class.
		///
		static Log& i();

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
		void log(const pl::Log::Level level, const std::string& message);

		///
		/// Convert log message level to a string.
		///
		/// \param level Level to convert.
		///
		/// \return std::string, in caps.
		///
		std::string processLevel(const pl::Log::Level level);

		///
		/// Colourizes the terminal text based on the log message level.
		///
		/// \param level Level to use when selecting colour.
		///
		/// \return Colour code in std::string on Unix, std::blank string on Windows (set via console library).
		///
		std::string processColour(pl::Log::Level level);
		
		///
		/// Filters a log stream message based on message level to determine if it must be logged.
		///
		/// \param level Level of current message to determine if it must be logged.
		///
		/// \return True if can log.
		///
		bool filterLevel(pl::Log::Level level);
		
		///
		/// \brief	Set a minimum log level.
		///
		/// In order to only print and log levels greater than or equal to the current log message level.
		///
		/// \param level Level to set as the minimum level to log at.
		///
		void setMinimumLevel(pl::Log::Level level);
		
		///
		/// Returns minimum logging message level that is required to log a message.
		///
		/// \return pl::Log::Level enum.
		///
		pl::Log::Level getMinimumLevel();
		
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
		/// File stream to write to.
		///
		std::ofstream m_fileStream;

		///
		/// Minimum level of messages required to be logged.
		///
		pl::Log::Level m_minimumLevel;

		///
		/// Thread callback function.
		///
		std::function<void(const pl::Log::Level, const std::string&)> m_callback;
	};
}

#endif