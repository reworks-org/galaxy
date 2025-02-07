///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_LOGGING_LOGLEVEL_HPP_
#define GALAXY_LOGGING_LOGLEVEL_HPP_

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
			_INFO_ = 0,

			///
			/// Debug Log Level.
			///
			_DEBUG_ = 1,

			///
			/// Warning Log Level.
			///
			_WARNING_ = 2,

			///
			/// Error Log Level.
			///
			_ERROR_ = 3,

			///
			/// Fatal Log Level.
			///
			_FATAL_ = 4
		};
	} // namespace logging
} // namespace galaxy

#endif
