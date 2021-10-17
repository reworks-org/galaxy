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
			/// \brief Error Log Level.
			///
			/// Has an underscore to prevent conflicts with some macro definitions in Windows.
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