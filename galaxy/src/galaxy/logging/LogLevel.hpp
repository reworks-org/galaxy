///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOGLEVEL_HPP_
#define GALAXY_LOGGING_LOGLEVEL_HPP_

#undef ERROR

namespace galaxy
{
	namespace logging
	{
		///
		/// Used to determine filtering and colouring of log messages.
		///
		enum class LogLevel : int
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
			ERROR = 3,

			///
			/// Fatal Log Level.
			///
			FATAL = 4
		};
	} // namespace logging
} // namespace galaxy

#endif
