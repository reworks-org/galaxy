///
/// Keyboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYBOARD_HPP_
#define GALAXY_INPUT_KEYBOARD_HPP_

#include <robin_hood.h>

#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Physical keyboard device and state management.
		///
		class Keyboard final
		{
		public:
			///
			/// Constructor.
			///
			Keyboard() noexcept;

			///
			/// Move constructor.
			///
			Keyboard(Keyboard&&) noexcept;

			///
			/// Move assignment operator.
			///
			Keyboard& operator=(Keyboard&&) noexcept;

			///
			/// Copy constructor.
			///
			Keyboard(const Keyboard&) noexcept;

			///
			/// Copy assignment operator.
			///
			Keyboard& operator=(const Keyboard&) noexcept;

			///
			/// Destructor.
			///
			~Keyboard() noexcept;

		public:
			///
			/// Map of galaxy keys to GLFW key codes.
			///
			robin_hood::unordered_flat_map<Keys, int> m_keymap;

			///
			/// Map of GLFW key codes to galaxy keys.
			///
			robin_hood::unordered_flat_map<int, Keys> m_reverse_keymap;

			///
			/// Previous key states.
			///
			robin_hood::unordered_flat_map<Keys, int> m_prev_key_states;

			///
			/// String for text input.
			///
			std::string m_text_input;

			///
			/// Flag to signal to glfw that text is being input.
			///
			bool m_inputting_text;
		};
	} // namespace input
} // namespace galaxy

#endif