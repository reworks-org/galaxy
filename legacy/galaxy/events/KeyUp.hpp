///
/// KeyUp.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYUP_HPP_
#define GALAXY_EVENTS_KEYUP_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was released.
		///
		struct KeyUp final
		{
			///
			/// Default constructor.
			///
			KeyUp() noexcept = default;

			///
			/// Constructor.
			///
			/// \param keycode Keycode for released key.
			/// \param mod Modifier code.
			///
			KeyUp(const input::Keys keycode, const input::KeyMod mod) noexcept;

			///
			/// Default destructor.
			///
			~KeyUp() noexcept = default;

			///
			/// Keycode for the key that was released.
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