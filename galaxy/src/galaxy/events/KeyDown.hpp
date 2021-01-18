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
			///
			KeyDown(const input::Keys keycode) noexcept;

			///
			/// Default destructor.
			///
			~KeyDown() noexcept = default;

			///
			/// Keycode for the key that was pressed.
			///
			input::Keys m_keycode;
		};
	} // namespace events
} // namespace galaxy

#endif