///
/// KeyRepeat.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYREPEAT_HPP_
#define GALAXY_EVENTS_KEYREPEAT_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct KeyRepeat final
		{
			///
			/// Default constructor.
			///
			KeyRepeat() noexcept = default;

			///
			/// Constructor.
			///
			/// \param keycode Keycode for the key that is repeating.
			///
			KeyRepeat(const input::Keys keycode) noexcept;

			///
			/// Default destructor.
			///
			~KeyRepeat() noexcept = default;

			///
			/// Keycode for the key that is repeating.
			///
			input::Keys m_keycode;
		};
	} // namespace events
} // namespace galaxy

#endif