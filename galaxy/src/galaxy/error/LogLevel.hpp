///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOGLEVEL_HPP_
#define GALAXY_ERROR_LOGLEVEL_HPP_

#include <type_traits>

#undef ERROR

namespace galaxy
{
	namespace error
	{
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

		struct Info
		{
			static inline constexpr auto value()
			{
				return Level::INFO;
			}
		};

		struct Debug
		{
			static inline constexpr auto value()
			{
				return Level::DEBUG;
			}
		};

		struct Warning
		{
			static inline constexpr auto value()
			{
				return Level::WARNING;
			}
		};

		struct Error
		{
			static inline constexpr auto value()
			{
				return Level::ERROR;
			}
		};

		struct Fatal
		{
			static inline constexpr auto value()
			{
				return Level::FATAL;
			}
		};

		///
		/// Log Level concept.
		///
		template<typename Type>
		concept type = std::is_same<Info, Type>::value || std::is_same<Debug, Type>::value || std::is_same<Warning, Type>::value || std::is_same<Error, Type>::value || std::is_same<Fatal, Type>::value;
	} // namespace error
} // namespace galaxy

#endif