///
/// Input.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUT_HPP_
#define GALAXY_INPUT_INPUT_HPP_

#include <glm/vec2.hpp>

#include "galaxy/input/Keys.hpp"
#include "galaxy/input/MouseButton.hpp"

namespace galaxy
{
	namespace input
	{
		///
		/// See if a key mod is being held down.
		///
		/// \param modifier Keymod to check.
		///
		/// \return True if keymod is currently down.
		///
		[[nodiscard]]
		bool is_key_mod_down(const KeyMods modifier) noexcept;

		///
		/// See if a key is being held down.
		///
		/// \param key Key to check.
		///
		/// \return True if key is currently down.
		///
		[[nodiscard]]
		bool is_key_down(const Keys key) noexcept;

		///
		/// Check if a mouse button was pressed.
		///
		/// \param btn Mouse button keycode.
		///
		/// \return True if button was pressed.
		///
		[[nodiscard]]
		bool is_mouse_down(const MouseButton btn) noexcept;

		///
		/// Get current cursor position.
		///
		/// \return Returns position as a vec2.
		///
		[[nodiscard]]
		glm::vec2 get_cursor_pos() noexcept;
	} // namespace input
} // namespace galaxy

#endif
