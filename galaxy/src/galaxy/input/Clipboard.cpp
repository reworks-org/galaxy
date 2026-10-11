///
/// Clipboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_clipboard.h>

#include "Clipboard.hpp"

namespace galaxy
{
	namespace input
	{
		void set_clipboard(const std::string& contents) noexcept
		{
			SDL_SetClipboardText(contents.c_str());
		}

		std::string get_clipboard() noexcept
		{
			if (SDL_HasClipboardText())
			{
				std::string text {SDL_GetClipboardText()};
				return text;
			}
			else
			{
				return {};
			}
		}
	} // namespace input
} // namespace galaxy
