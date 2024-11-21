///
/// Cursor.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_INPUT_CURSOR_HPP_
#define GALAXY_INPUT_CURSOR_HPP_

#include <raylib.h>

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
			void show();

			///
			/// Hide cursor.
			///
			void hide();

			///
			/// Check if cursor is not visible.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool is_hidden();

			///
			/// Enables cursor (unlock cursor).
			///
			void enable();

			///
			/// Disables cursor (lock cursor).
			///
			void disable();

			///
			/// Check if cursor is on the screen.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool on_screen();

			///
			/// Set mouse cursor to a OS provided cursor.
			///
			/// \param cursor MouseCursor enum from available in OS.
			///
			void set(const MouseCursor cursor);
		} // namespace cursor
	} // namespace input
} // namespace galaxy

#endif
