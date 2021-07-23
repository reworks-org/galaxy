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
			/// \param mod Modifier code.
			///
			KeyRepeat(const input::Keys keycode, const input::KeyMod mod) noexcept;

			///
			/// Default destructor.
			///
			~KeyRepeat() noexcept = default;

			///
			/// Keycode for the key that is repeating.
			///
			input::Keys m_keycode;

			///
			/// Modifier Key.
			///
			input::KeyMod m_mod;
		};
	} // namespace events
} // namespace galaxy

#endif