///
/// Log.hpp
///
/// pulsar
/// See LICENSE.txt.
///

#ifndef PULSAR_LOG_HPP_
#define PULSAR_LOG_HPP_

#include "platform/Unix.hpp"
#include "platform/Windows.hpp"
#include "details/LogStream.hpp"

/*
https://stackoverflow.com/questions/11711920/how-to-implement-multithread-safe-Log-in-c11-without-using-mutex
https://en.cppreference.com/w/cpp/thread/async
https://gist.github.com/kevinkreiser/39f2e39273c625d96790
https://hackernoon.com/learn-c-multi-threading-in-5-minutes-8b881c92941f
https://github.com/gabime/spdlog
*/

///
/// \brief Log to Stream.
///
/// Macro shortcut.
///
#define LOG_S(x) pl::Log::i().filterLevel(x) << pl::Log::i().processColour(x) << "[" << pl::Log::i().processLevel(x) << "] - " << pl::Log::i().getDateTime() << " - "

///
/// pl is the namespace for the Pulsar Logging library.
///
namespace pl
{
	///
	/// Log logging class.
	/// Uses multithreading.
	///
	class Log
	{
	public:
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
		/// Convert log message level to a string.
		///
		/// \param level Level to convert.
		///
		/// \return C-String, in caps.
		///
		const char* processLevel(const pl::Log::Level level);

		///
		/// Colourizes the terminal text based on the log message level.
		///
		/// \param level Level to use when selecting colour.
		///
		/// \return Colour code in C-String on Unix, blank on Windows (set via console library).
		///
		const char* processColour(pl::Log::Level level);
		
		///
		/// Filters a log stream message based on message level to determine if it must be logged.
		///
		/// \param level Level of current message to determine if it must be logged.
		///
		/// \return Returns a Logging Stream object that can be used in a log to stream senario.
		///
		pl::LogStream& filterLevel(pl::Log::Level level);
		
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
		/// \return Returns date/time as a C-String.
		///
		const char* getDateTime();

		///
		/// Retrieves currently used logging stream.
		///
		/// \return Reference to LogStream object.
		///
		pl::LogStream& stream();

	private:
		///
		/// Constructor.
		///
		Log();

		///
		/// Destructor.
		///
		~Log() = default;

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
		std::mutex m_lock;
		pl::LogStream m_stream;
		pl::LogStream m_emptyStream;
		pl::Log::Level m_minimumLevel;
	};
}

#endif