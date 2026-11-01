///
/// Mouse.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_MOUSE_HPP_
#define GALAXY_INPUT_MOUSE_HPP_

#include <glm/vec2.hpp>
#include <SDL3/SDL_mouse.h>

#include "galaxy/input/MouseButton.hpp"
#include "galaxy/math/Rect.hpp"
#include "galaxy/meta/WindowBindable.hpp"

namespace sl
{
	struct Bitmap;
}

namespace galaxy
{
	///
	/// Physical mouse device and state management.
	///
	class Mouse final : public WindowBindable
	{
	public:
		///
		/// Constructor.
		///
		Mouse() noexcept;

		///
		/// Destructor.
		///
		~Mouse() noexcept;

		///
		/// Check if there is a mouse connected.
		///
		/// \return True if there is a mouse device connected.
		///
		[[nodiscard]]
		bool mouse_connected() const noexcept;

		///
		/// Toggle mouse grab.
		///
		/// \param grabbed This is true to grab keyboard, and false to release.
		///
		void set_mouse_grab(const bool grabbed) const noexcept;

		///
		/// \brief Move the mouse cursor to the given position within the window.
		///
		/// This function generates a mouse motion event.
		/// Will not work correctly if mouse space is altered.
		///
		/// \param x The x coordinate within the window.
		/// \param y The y coordinate within the window.
		///
		void warp_mouse(const float x, const float y) const noexcept;

		///
		/// \brief Confines the cursor to the specified area of a window.
		///
		/// Note that this does NOT grab the cursor, it only defines the area a cursor is restricted to when the window has mouse focus.
		/// Will not work correctly if mouse space is altered.
		///
		/// \param rect A rectangle area in window-relative coordinates.
		///
		/// \return True on success.
		///
		[[nodiscard]]
		bool set_window_mouse_rect(iRect& rect) noexcept;

		///
		/// Show cursor.
		///
		void show_cursor() const noexcept;

		///
		/// Hide cursor.
		///
		void hide_cursor() const noexcept;

		///
		/// Set cursor to a system cursor.
		///
		/// \param cursor SDL_SystemCursor enum.
		///
		void set_cursor_system(const SDL_SystemCursor cursor) noexcept;

		///
		/// \brief Replaces the system cursor with a bitmap.
		///
		/// Use a bitmap as the mouse cursor, drawn so the pixel at (offset_x, offset_y), measured from the bitmap's top-left, sits on the mouse position; nullptr restores the OS.
		///
		/// \param cursor Texture to draw.
		/// \param offset Top left, right of mouse cursor.
		///
		void use_custom_mouse(const std::string& cursor, const glm::ivec2& offset = glm::ivec2(0, 0));

		///
		/// Set the cursor back to default.
		///
		void restore_cursor() noexcept;

		///
		/// Check if a mouse button was pressed.
		///
		/// \param btn Mouse button keycode.
		///
		/// \return True if button was pressed.
		///
		[[nodiscard]]
		bool is_mouse_down(const MouseButton btn) const noexcept;

		///
		/// Get current cursor x position.
		///
		/// \return Returns logical pixel pos.
		///
		[[nodiscard]]
		int mouse_x() const noexcept;

		///
		/// Get current cursor y position.
		///
		/// \return Returns logical pixel pos.
		///
		[[nodiscard]]
		int mouse_y() const noexcept;

		///
		/// \brief Set mouse relative mapping to another coord space.
		///
		/// Make mouse_x()/mouse_y() report positions inside a scaled image drawn at (x, y) in screen coordinates with the
		/// given scale, for example a letterboxed or integer-scaled render target. See present_scaled_target().
		/// Positions over the bars fall outside the image's coordinate range.
		///
		/// \param x Upper X position of space.
		/// \param y Upper Y position of space.
		/// \param scale_x Scale "width".
		/// \param scale_y Scale "height".
		///
		void set_mouse_mapping(const int x, const int y, const float scale_x, const float scale_y) const noexcept;

		///
		/// Return mouse_x()/mouse_y() to plain screen coordinates.
		///
		void clear_mouse_mapping() const noexcept;

		///
		/// Enable or disable relative mouse mode (locks cursor to window, hides it, and reports relative motion).
		///
		/// \param enabled True to enable, false to disable.
		///
		void set_relative_mouse_mode(const bool enabled) const noexcept;

		///
		/// Return whether relative mouse mode is currently enabled.
		///
		/// \return True if the mouse is in relative mode.
		///
		[[nodiscard]]
		bool relative_mouse_mode() const noexcept;

		///
		/// Retrieve relative mouse movement (delta) since the last call.
		///
		/// \return vec2, x is delta x, y is delta y.
		///
		[[nodiscard]]
		glm::ivec2 mouse_delta(int* dx, int* dy) noexcept;

	private:
		///
		/// Move constructor.
		///
		Mouse(Mouse&&) = delete;

		///
		/// Move assignment operator.
		///
		Mouse& operator=(Mouse&&) = delete;

		///
		/// Copy constructor.
		///
		Mouse(const Mouse&) = delete;

		///
		/// Copy assignment operator.
		///
		Mouse& operator=(const Mouse&) = delete;

		///
		/// Destroy any existing cursor.
		///
		void destroy_cursor() noexcept;

	private:
		///
		/// Holds data for a custom cursor.
		///
		SDL_Cursor* m_cursor;

		///
		/// Mouse custom bitmap.
		///
		sl::Bitmap* m_cursor_bitmap;
	};
} // namespace galaxy

#endif
