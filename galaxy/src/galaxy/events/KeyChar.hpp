///
/// KeyChar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYCHAR_HPP_
#define GALAXY_EVENTS_KEYCHAR_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		///  for use with unicode characters, not KeyDown, etc...
		///
		struct KeyChar final
		{
			///
			/// Default constructor.
			///
			KeyChar();

			///
			/// Argument constructor.
			///
			/// \param keycode Code of the key pressed.
			/// \param unichar Unicode character pressed.
			///
			KeyChar(const input::Keys keycode, const int unichar);

			///
			/// Default destructor.
			///
			~KeyChar() = default;

			///
			/// Keycode for the key that was pressed.
			///
			input::Keys m_keycode;

			///
			/// Unicode character for use with unicode strings.
			///
			int m_unichar;
		};
	} // namespace events
} // namespace galaxy

#endif