///
/// Keyboard.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_KEYBOARD_HPP_
#define GALAXY_INPUT_KEYBOARD_HPP_

#include <string>

#include <SDL3/SDL_keyboard.h>

#include "galaxy/meta/WindowBindable.hpp"

namespace galaxy
{
	///
	/// Physical keyboard device and state management.
	///
	class Keyboard final : public WindowBindable
	{
		friend class Window;

	public:
		///
		/// Destructor.
		///
		~Keyboard() noexcept;

		///
		/// \brief Enable window to begin processing text input.
		///
		/// Prevents normal key input until disabled.
		///
		/// \param input_type An SDL_TextInputType value that describes text being input.
		/// \param capitals An SDL_Capitalization value that describes how text should be capitalized.
		/// \param multiline True if multiple lines of text are allowed.
		///
		void begin_text_input(const SDL_TextInputType input_type, const SDL_Capitalization capitals, const bool multiline) const noexcept;

		///
		/// Disable text input.
		///
		void end_text_input() const noexcept;

		///
		/// Will clear any existing keyboard state.
		///
		void clear_state() const noexcept;

		///
		/// Check if there is a keyboard connected.
		///
		/// \return True if there is a keyboard connected.
		///
		[[nodiscard]]
		bool has_keyboard() const noexcept;

		///
		/// Check if an onscreen keyboard is supported.
		///
		/// \return True if supported.
		///
		[[nodiscard]]
		bool has_onscreen_keyboard() const noexcept;

		///
		/// Check if onscreen keyboard is shown.
		///
		/// \return True if onscreen keyboard is active.
		///
		[[nodiscard]]
		bool onscreen_keyboard_active() const noexcept;

	private:
		///
		/// Constructor.
		///
		Keyboard() noexcept;

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
	};
} // namespace galaxy

/*
* ///
		/// Flag to keep track of text input state.
		///
		bool m_text_input_enabled;


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
		std::string get_scancode_name(const int scancode) const;*/

#endif
