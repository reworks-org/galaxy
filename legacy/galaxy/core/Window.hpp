///
/// Window.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_WINDOW_HPP_
#define GALAXY_CORE_WINDOW_HPP_

#include <variant>
#include <queue>

#include <GLFW/glfw3.h>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/WindowSettings.hpp"
#include "galaxy/events/Dispatcher.hpp"
#include "galaxy/events/KeyDown.hpp"
#include "galaxy/events/KeyRepeat.hpp"
#include "galaxy/events/KeyUp.hpp"
#include "galaxy/events/KeyChar.hpp"
#include "galaxy/events/MouseMoved.hpp"
#include "galaxy/events/MousePressed.hpp"
#include "galaxy/events/MouseReleased.hpp"
#include "galaxy/events/MouseWheel.hpp"
#include "galaxy/events/WindowResized.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/PostProcessor.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/input/Cursor.hpp"
#include "galaxy/input/Keyboard.hpp"
#include "galaxy/input/Mouse.hpp"

namespace galaxy
{
	///
	/// Event type.
	///
	using Event = std::variant<std::monostate,
		events::KeyDown,
		events::KeyRepeat,
		events::KeyUp,
		events::KeyChar,
		events::MouseMoved,
		events::MousePressed,
		events::MouseReleased,
		events::MouseWheel,
		events::WindowResized>;

	///
	/// Event queue type.
	///
	using EventQueue = std::queue<Event>;

	namespace graphics
	{
		class Renderer;
	} // namespace graphics

	namespace core
	{
		///
		/// Represents a window to open, process and draw to.
		///
		class Window final
		{
		public:
			///
			/// Set window icon.
			///
			/// \param icon Icon to load.
			///
			void set_icon(std::string_view icon);

			///
			/// Set window icon.
			///
			/// \param buffer Memory buffer containing pixels.
			///
			void set_icon(std::span<unsigned char> buffer);

			

			///
			/// Set cursor visibility.
			///
			/// \param visible True for cursor to be visible, otherwise false.
			///
			void set_cursor_visibility(const bool visible) noexcept;

			///
			/// Set cursor icon.
			///
			/// \param icon Icon to load.
			///
			void set_cursor_icon(std::string_view icon);

			///
			/// Set cursor icon.
			///
			/// \param buffer Memory buffer containing pixels.
			///
			void set_cursor_icon(std::span<unsigned char> buffer);

			

			///
			/// Resizes window.
			///
			/// \param width Width of the window.
			/// \param height Height of the window.
			///
			void resize(const int width, const int height);

			

			///
			/// Prepare renderers to render.
			///
			void begin();

			///
			/// Draw renderer contents to screen.
			///
			void end();

			///
			/// Poll for events.
			///
			void poll_events() noexcept;

			///
			/// \brief See if a key is being held down.
			///
			/// This will pick up repeated events.
			///
			/// \param key Key to check.
			///
			/// \return True if key is currently down.
			///
			[[nodiscard]] const bool key_down(input::Keys key) noexcept;

			///
			/// Check if a key was pressed once.
			///
			/// \param key Key to check.
			///
			/// \return True if key was pressed once.
			///
			[[nodiscard]] const bool key_pressed(input::Keys key) noexcept;

			///
			/// Check if a mouse button was pressed.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a press occured.
			///
			[[nodiscard]] const bool mouse_button_pressed(input::MouseButtons mouse_button) noexcept;

			///
			/// Check if a mouse button was released.
			///
			/// \param mouse_button mouse button keycode.
			///
			/// \return True if a release occured.
			///
			[[nodiscard]] const bool mouse_button_released(input::MouseButtons mouse_button) noexcept;

			///
			/// Trigger all queued events for a dispatcher.
			///
			/// \param dispatcher An event dispatcher usually belonging to a scene.
			///
			void trigger_queued_events(events::Dispatcher& dispatcher);

			///
			/// Get mouse wheel delta.
			///
			/// \return Mouse scroll wheel delta.
			///
			[[nodiscard]] const double get_scroll_delta() noexcept;

			///
			/// Get current cursor position.
			///
			/// \return Returns cursor position as a glm::vec2.
			///
			[[nodiscard]] glm::vec2 get_cursor_pos() noexcept;

			///
			/// Get queued events.
			///
			/// \return Reference to EventQueue.
			///
			[[nodiscard]] EventQueue& queued_events() noexcept;

			

			///
			/// Get cursor size.
			///
			/// \return Const glm::vec2.
			///
			[[nodiscard]] const glm::vec2& cursor_size() const noexcept;

		private:
			///
			/// Post processor.
			///
			std::unique_ptr<graphics::PostProcessor> m_post_processor;

			///
			/// Custom cursor.
			///
			input::Cursor m_cursor;

			///
			/// Custom keyboard.
			///
			input::Keyboard m_keyboard;

			///
			/// Custom mouse.
			///
			input::Mouse m_mouse;

			///
			/// Event queue.
			///
			EventQueue m_event_queue;
		};
	} // namespace core
} // namespace galaxy

#endif