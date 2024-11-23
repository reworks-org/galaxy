///
/// Cursor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CURSOR_HPP_
#define GALAXY_INPUT_CURSOR_HPP_

#include <raylib.hpp>

namespace galaxy
{
	namespace input
	{
		///
		/// Mouse cursor state management.
		///
		namespace cursor
		{
			///
			/// Show cursor.
			///
			void show() noexcept;

			///
			/// Hide cursor.
			///
			void hide() noexcept;

			///
			/// Check if cursor is not visible.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_hidden() noexcept;

			///
			/// Enables cursor (unlock cursor).
			///
			void enable() noexcept;

			///
			/// Disables cursor (lock cursor).
			///
			void disable() noexcept;

			///
			/// Check if cursor is on the screen.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool on_screen() noexcept;

			///
			/// Set mouse cursor to a OS provided cursor.
			///
			/// \param cursor MouseCursor enum from available in OS.
			///
			void set(const ray::MouseCursor cursor) noexcept;
		} // namespace cursor
	} // namespace input
} // namespace galaxy

#endif
