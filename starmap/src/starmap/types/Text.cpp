///
/// Text.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include "nlohmann/json.hpp"

#include "Text.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Text::Text() noexcept
		:m_bold(false), m_color(""), m_fontFamily(""), m_hAlign(""), m_italic(false), m_kerning(false), m_pixelSize(0), m_strikeOut(false), m_text(""), m_underLine(false), m_vAlign(""), m_wrap(false)
	{
	}

	Text::Text(const nlohmann::json& json) noexcept
	{
	}
}