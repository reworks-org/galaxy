///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Theme.hpp"

namespace galaxy
{
	namespace ui
	{
		UITheme::UITheme()
		    : m_fonts {nullptr}, m_shaders {nullptr}, m_window {nullptr}, m_renderer {nullptr}, m_font_col {255, 255, 255, 255}
		{
		}
	} // namespace ui
} // namespace galaxy