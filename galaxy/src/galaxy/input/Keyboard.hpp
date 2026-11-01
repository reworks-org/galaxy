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

#include "galaxy/input/Keys.hpp"
#include "galaxy/meta/WindowBindable.hpp"

namespace galaxy
{
	///
	/// Physical keyboard device and state management.
	///
	class Keyboard final : public WindowBindable
	{
	public:
		///
		/// Constructor.
		///
		Keyboard() noexcept;

		///
		/// Destructor.
		///
		~Keyboard() noexcept;

		///
		/// \brief Enable window to begin processing text input.
		///
		/// Prevents normal key input until disabled. Triggers SDL_EVENT_TEXT_INPUT and SDL_EVENT_TEXT_EDITING.
		///
		/// \param input_type An SDL_TextInputType value that describes text being input.
		/// \param capitals An SDL_Capitalization value that describes how text should be capitalized.
		/// \param multiline True if multiple lines of text are allowed.
		///
		void begin_text_input(const SDL_TextInputType input_type, const SDL_Capitalization capitals, const bool multiline) const noexcept;

		///
		/// Is text input active?
		///
		/// \return True if in text input mode.
		///
		[[nodiscard]]
		bool is_text_input_active() const noexcept;

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

		///
		/// See if a key mod is being held down.
		///
		/// \param modifier Keymod to check.
		///
		/// \return True if keymod is currently down.
		///
		[[nodiscard]]
		bool is_key_mod_down(const KeyMods modifier) const noexcept;

		///
		/// See if a key is being held down.
		///
		/// \param key Key to check.
		///
		/// \return True if key is currently down.
		///
		[[nodiscard]]
		bool is_key_down(const Keys key) const noexcept;

		///
		/// Get key scancode.
		///
		/// \param key Key to query.
		/// \param modifier Keymod to check.
		///
		/// \return Platform-specific scancode of a key.
		///
		[[nodiscard]]
		SDL_Scancode get_scancode(const Keys key, const KeyMods modifier = KeyMods::MOD_NONE) const noexcept;

	private:
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
	};
} // namespace galaxy

#endif
