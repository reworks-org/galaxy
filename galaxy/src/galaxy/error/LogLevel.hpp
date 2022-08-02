///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOGLEVEL_HPP_
#define GALAXY_ERROR_LOGLEVEL_HPP_

#undef ERROR

namespace galaxy
{
	namespace error
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
	} // namespace error
} // namespace galaxy

#endif