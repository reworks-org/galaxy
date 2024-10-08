///
/// LogMessage.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOGMESSAGE_HPP_
#define GALAXY_LOGGING_LOGMESSAGE_HPP_

#include <string>

namespace galaxy
{
	namespace logging
	{
		///
		/// Parts of a log message to be passed to sinks.
		///
		struct LogMessage final
		{
			///
			/// ASCII colour code.
			///
			std::string colour;

			///
			/// Level of message.
			///
			std::string level;

			///
			/// Timestamp of message.
			///
			std::string time;

			///
			/// File message occured in.
			///
			std::string file;

			///
			/// Line the message occured on.
			///
			std::string line;

			///
			/// The actual message to record.
			///
			std::string message;

			///
			/// Stack trace.
			///
			std::string trace;
		};
	} // namespace logging
} // namespace galaxy

#endif
