///
/// Keyboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYBOARD_HPP_
#define GALAXY_INPUT_KEYBOARD_HPP_

#include <string>

#include "galaxy/input/InputDevice.hpp"
#include "galaxy/input/Keys.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// Physical keyboard device and state management.
		///
		class Keyboard final : public InputDevice
		{
			friend class core::Window;

		public:
			///
			/// Destructor.
			///
			virtual ~Keyboard();

			///
			/// \brief Enable window to begin processing text input.
			///
			/// Prevents normal key input until disabled.
			///
			void begin_text_input();

			///
			/// Disable text input.
			///
			void end_text_input();

			///
			/// \brief Enable sticky keys.
			///
			/// The pollable state of a key will remain pressed until the state of that key is polled.
			/// This applies to modifer keys aswell, such as CAPS LOCK.
			///
			void enable_sticky_keys() const;

			///
			/// Disable sticky keys.
			///
			void disable_sticky_keys() const;

			///
			/// Check if window is in text input mode.
			///
			/// \return True if only text input is being registered.
			///
			[[nodiscard]]
			bool is_text_input_enabled() const;

			///
			/// Get key scancode.
			///
			/// \param key Key to query.
			///
			/// \return Platform-specific scancode of a key.
			///
			[[nodiscard]]
			int get_scancode(const input::Keys key) const;

			///
			/// Get key name.
			///
			/// \param key Key to query.
			///
			/// \return Name of key, empty if error occurs.
			///
			[[nodiscard]]
			std::string get_key_name(const input::Keys key) const;

			///
			/// Get key name.
			///
			/// \param scancode Scancode to query.
			///
			/// \return Name of scancode, empty if error occurs.
			///
			[[nodiscard]]
			std::string get_scancode_name(const int scancode) const;

		private:
			///
			/// Constructor.
			///
			Keyboard();

			///
			/// Move constructor.
			///
			Keyboard(Keyboard&&) = delete;

			///
			/// Move assignment operator.
			///
			Keyboard& operator=(Keyboard&&) = delete;

			///
			/// Copy constructor.
			///
			Keyboard(const Keyboard&) = delete;

			///
			/// Copy assignment operator.
			///
			Keyboard& operator=(const Keyboard&) = delete;

		private:
			///
			/// Flag to keep track of text input state.
			///
			bool m_text_input_enabled;
		};
	} // namespace input
} // namespace galaxy

#endif
