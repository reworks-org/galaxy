///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOGLEVEL_HPP_
#define GALAXY_ERROR_LOGLEVEL_HPP_

namespace galaxy
{
	namespace error
	{
		///
		/// Different levels of error messaging.
		///
		enum class LogLevel : short
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
			ERROR_ = 3,

			///
			/// Fatal Log Level.
			///
			FATAL = 4
		};
	} // namespace error
} // namespace galaxy

#endif