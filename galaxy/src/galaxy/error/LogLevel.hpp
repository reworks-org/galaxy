///
/// LogLevel.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ERROR_LOGLEVEL_HPP_
#define GALAXY_ERROR_LOGLEVEL_HPP_

#include <type_traits>

namespace galaxy
{
	namespace error
	{
		namespace level
		{
			///
			/// Info Log Level.
			///
			struct Info
			{
				[[nodiscard]] static inline constexpr auto value()
				{
					return 0;
				}
			};

			///
			/// Debug Log Level.
			///
			struct Debug
			{
				[[nodiscard]] static inline constexpr auto value()
				{
					return 1;
				}
			};

			///
			/// Warning Log Level.
			///
			struct Warning
			{
				[[nodiscard]] static inline constexpr auto value()
				{
					return 2;
				}
			};

			///
			/// Error Log Level.
			///
			struct Error
			{
				[[nodiscard]] static inline constexpr auto value()
				{
					return 3;
				}
			};

			///
			/// Fatal Log Level.
			///
			struct Fatal
			{
				[[nodiscard]] static inline constexpr auto value()
				{
					return 4;
				}
			};
		} // namespace level

		///
		/// Log Level requirement concept.
		///
		template<typename Type>
		concept loglevel_type = std::is_same<level::Info, Type>::value || std::is_same<level::Debug, Type>::value || std::is_same<level::Warning, Type>::value || std::is_same<level::Error, Type>::value || std::is_same<level::Fatal, Type>::value;
	} // namespace error
} // namespace galaxy

#endif