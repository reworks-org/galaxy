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

#include "galaxy/meta/WindowBindable.hpp"

namespace galaxy
{
	///
	/// Physical mouse device and state management.
	///
	class Mouse final : public WindowBindable
	{
		friend class Window;

	public:
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
		bool has_mouse() const noexcept;

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
		///
		/// \param x The x coordinate within the window.
		/// \param y The y coordinate within the window.
		///
		void warp_mouse(const float x, const float y) const noexcept;

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
		/// Set custom cursor texture.
		///
		/// \param cursor Cursor texture in VFS.
		/// \param hotspot The position of the cursor hot spot where the click is triggered.
		///
		void set_cursor_custom(const std::string& cursor, const glm::ivec2& hotspot) noexcept;

		///
		/// Set the cursor back to default.
		///
		void restore_cursor() noexcept;

	private:
		///
		/// Constructor.
		///
		Mouse() noexcept;

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
	};
} // namespace galaxy

#endif
