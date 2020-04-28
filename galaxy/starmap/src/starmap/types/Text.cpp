///
/// Text.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

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

	void Text::parse(const nlohmann::json& json) noexcept
	{
		if (json.at("bold") > 0)
		{
			m_bold = json.at("bold");
		}

		if (json.at("color") > 0)
		{
			m_color = json.at("color");
		}

		if (json.at("fontfamily") > 0)
		{
			m_fontFamily = json.at("fontfamily");
		}

		if (json.at("halign") > 0)
		{
			m_hAlign = json.at("halign");
		}

		if (json.at("italic") > 0)
		{
			m_italic = json.at("italic");
		}

		if (json.at("kerning") > 0)
		{
			m_kerning = json.at("kerning");
		}

		if (json.at("pixelsize") > 0)
		{
			m_pixelSize = json.at("pixelsize");
		}

		if (json.at("strikeout") > 0)
		{
			m_strikeOut = json.at("strikeout");
		}

		if (json.at("text") > 0)
		{
			m_text = json.at("text");
		}

		if (json.at("underline") > 0)
		{
			m_underLine = json.at("underline");
		}

		if (json.at("valign") > 0)
		{
			m_vAlign = json.at("valign");
		}

		if (json.at("wrap") > 0)
		{
			m_wrap = json.at("wrap");
		}
	}

	const bool Text::isBold() const noexcept
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

	const bool Text::isItalic() const noexcept
	{
		return m_italic;
	}

	const bool Text::isUsingKerning() const noexcept
	{
		return m_kerning;
	}

	const int Text::getPixelSize() const noexcept
	{
		return m_pixelSize;
	}

	const bool Text::isStrikedOut() const noexcept
	{
		return m_strikeOut;
	}

	const std::string& Text::getText() const noexcept
	{
		return m_text;
	}

	const bool Text::isUnderlined() const noexcept
	{
		return m_underLine;
	}

	const std::string& Text::getVAlign() const noexcept
	{
		return m_vAlign;
	}

	const bool Text::isWrapped() const noexcept
	{
		return m_wrap;
	}
}