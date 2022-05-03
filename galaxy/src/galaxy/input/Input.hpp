///
/// Input.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_INPUT_HPP_
#define GALAXY_INPUT_INPUT_HPP_

struct GLFWwindow;

#include "galaxy/input/Keys.hpp"
#include "galaxy/input/Mouse.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace input
	{
		class Input final
		{
			friend class core::Window;

		public:
			///
			/// \brief See if a key is being held down.
			///
			/// This will pick up repeated events.
			///
			/// \param key Key to check.
			///
			/// \return True if key is currently down.
			///
			[[nodiscard]] static bool key_down(input::Keys key) noexcept;

			///
			/// Check if a mouse button was pressed.
			///
			/// \param button mouse button keycode.
			///
			/// \return True if button was pressed.
			///
			[[nodiscard]] static bool mouse_button_down(input::Mouse::Buttons button) noexcept;

			///
			/// Get current cursor position.
			///
			/// \return Returns position as a const vec2 reference.
			///
			[[nodiscard]] static const glm::dvec2& get_cursor_pos() noexcept;

		private:
			///
			/// Constructor.
			///
			Input() = delete;

			///
			/// Move constructor.
			///
			Input(Input&&) = delete;

			///
			/// Move assignment operator.
			///
			Input& operator=(Input&&) = delete;

			///
			/// Copy constructor.
			///
			Input(const Input&) = delete;

			///
			/// Copy assignment operator.
			///
			Input& operator=(const Input&) = delete;

		private:
			///
			/// Static cursor pos cache.
			///
			static glm::dvec2 s_cursor_pos;

			///
			/// Pointer to glfw window.
			///
			static GLFWwindow* s_window;
		};
	} // namespace input
} // namespace galaxy

#endif