///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Theme.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	UITheme::UITheme()
	    : m_fonts {nullptr}, m_shaders {nullptr}, m_window {nullptr}, m_renderer {nullptr}, m_font_col {255, 255, 255, 255}
	{
	}

	UITheme::UITheme(UITheme&& uit)
	{
		this->m_atlas    = std::move(uit.m_atlas);
		this->m_fonts    = std::move(uit.m_fonts);
		this->m_font_col = std::move(uit.m_font_col);
		this->m_renderer = uit.m_renderer;
		this->m_shaders  = uit.m_shaders;
		this->m_window   = uit.m_window;
	}

	UITheme& UITheme::operator=(UITheme&& uit)
	{
		if (this != &uit)
		{
			this->m_atlas    = std::move(uit.m_atlas);
			this->m_fonts    = std::move(uit.m_fonts);
			this->m_font_col = std::move(uit.m_font_col);
			this->m_renderer = uit.m_renderer;
			this->m_shaders  = uit.m_shaders;
			this->m_window   = uit.m_window;
		}

		return *this;
	}
} // namespace galaxy