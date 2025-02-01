///
/// Clipboard.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Clipboard.hpp"

namespace galaxy
{
	namespace input
	{
		namespace clipboard
		{
			void set(const std::string& text) noexcept
			{
				::SetClipboardText(text.c_str());
			}

			std::string get() noexcept
			{
				return {::GetClipboardText()};
			}

			graphics::Image get_image() noexcept
			{
				return graphics::Image(::GetClipboardImage());
			}
		} // namespace clipboard
	} // namespace input
} // namespace galaxy
