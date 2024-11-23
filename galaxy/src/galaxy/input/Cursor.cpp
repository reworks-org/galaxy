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
			void show() noexcept
			{
				::ShowCursor();
			}

			void hide() noexcept
			{
				::HideCursor();
			}

			bool is_hidden() noexcept
			{
				return ::IsCursorHidden();
			}

			void enable() noexcept
			{
				::EnableCursor();
			}

			void disable() noexcept
			{
				::DisableCursor();
			}

			bool on_screen() noexcept
			{
				return ::IsCursorOnScreen();
			}

			void set(const ray::MouseCursor cursor) noexcept
			{
				::SetMouseCursor(static_cast<int>(cursor));
			}
		} // namespace cursor
	} // namespace input
} // namespace galaxy
