///
/// KeyDown.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYDOWN_HPP_
#define GALAXY_EVENTS_KEYDOWN_HPP_

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace events
	{
		///
		/// Contains the key that was pressed down.
		///
		struct KeyDown final
		{
			///
			/// Default constructor.
			///
			KeyDown() noexcept = default;

			///
			/// Constructor.
			///
			/// \param keycode Keycode for the key that was pressed.
			/// \param mod Modifier code.
			///
			KeyDown(const input::Keys keycode, const input::KeyMod mod) noexcept;

			///
			/// Default destructor.
			///
			~KeyDown() noexcept = default;

			///
			/// Keycode for the key that was pressed.
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