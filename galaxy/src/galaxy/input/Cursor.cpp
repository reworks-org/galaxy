///
/// Cursor.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Cursor.hpp"

namespace galaxy
{
	namespace input
	{
		namespace cursor
		{
			void show()
			{
				ShowCursor();
			}

			void hide()
			{
				HideCursor();
			}

			bool is_hidden()
			{
				return IsCursorHidden();
			}

			void enable()
			{
				EnableCursor();
			}

			void disable()
			{
				DisableCursor();
			}

			bool on_screen()
			{
				return IsCursorOnScreen();
			}

			void set(const MouseCursor cursor)
			{
				SetMouseCursor(static_cast<int>(cursor));
			}
		} // namespace cursor
	} // namespace input
} // namespace galaxy
