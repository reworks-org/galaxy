///
/// Clipboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <Raylib.hpp>

#include "Clipboard.hpp"

namespace galaxy
{
	namespace input
	{
		void set_clipboard(const std::string& contents) noexcept
		{
			ray::SetClipboardText(contents.c_str());
		}

		std::string get_clipboard() noexcept
		{
			return {ray::GetClipboardText()};
		}
	} // namespace input
} // namespace galaxy
