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
		:m_bold(false), m_color("00FFFFFF"), m_fontFamily(""), m_hAlign(""), m_italic(false), m_kerning(false), m_pixelSize(0), m_strikeOut(false), m_text(""), m_underLine(false), m_vAlign(""), m_wrap(false)
	{
	}

	Text::Text(const nlohmann::json& json) noexcept
		:m_bold(false), m_color("00FFFFFF"), m_fontFamily(""), m_hAlign(""), m_italic(false), m_kerning(false), m_pixelSize(0), m_strikeOut(false), m_text(""), m_underLine(false), m_vAlign(""), m_wrap(false)
	{
		parse(json);
	}

	void Text::parse(const nlohmann::json& json)
	{
		m_bold = json.at("bold");

		m_color = json.at("color");

		m_fontFamily = json.at("fontfamily");

		m_hAlign = json.at("halign");

		m_italic = json.at("italic");

		m_kerning = json.at("kerning");

		m_pixelSize = json.at("pixelsize");

		m_strikeOut = json.at("strikeout");

		m_text = json.at("text");

		m_underLine = json.at("underline");

		m_vAlign = json.at("valign");

		m_wrap = json.at("wrap");
	}

	const bool Text::getIsBold() const noexcept
	{
		return m_bold;
	}

	const std::string& Text::getColour() const noexcept
	{
		return m_color;
	}

	const std::string& Text::getFontFamily() const noexcept
	{
		return m_fontFamily;
	}

	const std::string& Text::getHAlign() const noexcept
	{
		return m_hAlign;
	}

	const bool Text::getIsItalic() const noexcept
	{
		return m_italic;
	}

	const bool Text::getUseKerning() const noexcept
	{
		return m_kerning;
	}

	const int Text::getPixelSize() const noexcept
	{
		return m_pixelSize;
	}

	const bool Text::getUseStrikeout() const noexcept
	{
		return m_strikeOut;
	}

	const std::string& Text::getText() const noexcept
	{
		return m_text;
	}

	const bool Text::getHasUnderline() const noexcept
	{
		return m_underLine;
	}

	const std::string& Text::getVAlign() const noexcept
	{
		return m_vAlign;
	}

	const bool Text::getIsWrapped() const noexcept
	{
		return m_wrap;
	}
}