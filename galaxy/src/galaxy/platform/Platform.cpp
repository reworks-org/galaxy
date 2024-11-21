///
/// Platform.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <raylib.h>

#include "Platform.hpp"

namespace galaxy
{
	namespace platform
	{
		void take_screenshot(const std::string& file)
		{
			TakeScreenshot(file.c_str());
		}

		void open_url(const std::string& url)
		{
			OpenURL(url.c_str());
		}
	} // namespace platform
} // namespace galaxy
