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
			void set(const std::string& text)
			{
				SetClipboardText(text.c_str());
			}

			std::string get()
			{
				return {GetClipboardText()};
			}

			Image get_image()
			{
				return GetClipboardImage();
			}
		} // namespace clipboard
	} // namespace input
} // namespace galaxy
