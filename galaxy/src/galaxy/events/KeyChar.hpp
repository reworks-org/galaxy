///
/// KeyChar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYCHAR_HPP_
#define GALAXY_EVENTS_KEYCHAR_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains UTF codepoint.
		///
		struct KeyChar final
		{
			///
			/// UTF codepoint.
			///
			unsigned int codepoint = 0u;

			///
			/// Has this event been handled?
			///
			bool handled = false;
		};
	} // namespace events
} // namespace galaxy

#endif
