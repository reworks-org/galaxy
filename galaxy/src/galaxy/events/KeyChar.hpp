///
/// KeyChar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYCHAR_HPP_
#define GALAXY_EVENTS_KEYCHAR_HPP_

#include <string>

namespace galaxy
{
	namespace events
	{
		///
		/// Contains UTF-8 char.
		///
		struct KeyChar final
		{
			///
			/// Unsigned integer representation.
			///
			unsigned int m_uichar = 0u;

			///
			/// List of entered character(s).
			///
			std::string m_char;

			///
			/// Has this event been handled?
			///
			bool m_handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif