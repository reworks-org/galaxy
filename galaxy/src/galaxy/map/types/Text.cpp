///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Text.hpp"

namespace galaxy
{
	namespace map
	{
		Text::Text()
		    : m_bold {false}, m_color {"00FFFFFF"}, m_font_family {""}, m_halign {""}, m_italic {false}, m_kerning {false}, m_pixel_size {0}, m_strike_out {false}, m_text {""}, m_underline {false}, m_valign {""}, m_wrap {false}
		{
		}

		Text::Text(const nlohmann::json& json)
		    : m_bold {false}, m_color {"00FFFFFF"}, m_font_family {""}, m_halign {""}, m_italic {false}, m_kerning {false}, m_pixel_size {0}, m_strike_out {false}, m_text {""}, m_underline {false}, m_valign {""}, m_wrap {false}
		{
			parse(json);
		}

		void Text::parse(const nlohmann::json& json)
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
				m_font_family = json.at("fontfamily");
			}

			if (json.at("halign") > 0)
			{
				m_halign = json.at("halign");
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
				m_pixel_size = json.at("pixelsize");
			}

			if (json.at("strikeout") > 0)
			{
				m_strike_out = json.at("strikeout");
			}

			if (json.at("text") > 0)
			{
				m_text = json.at("text");
			}

			if (json.at("underline") > 0)
			{
				m_underline = json.at("underline");
			}

			if (json.at("valign") > 0)
			{
				m_valign = json.at("valign");
			}

			if (json.at("wrap") > 0)
			{
				m_wrap = json.at("wrap");
			}
		}

		const bool Text::is_bold() const
		{
			return m_bold;
		}

		std::string Text::get_colour() const
		{
			return m_color;
		}

		std::string Text::get_font_family() const
		{
			return m_font_family;
		}

		std::string Text::get_h_align() const
		{
			return m_halign;
		}

		const bool Text::is_italic() const
		{
			return m_italic;
		}

		const bool Text::is_using_kerning() const
		{
			return m_kerning;
		}

		const int Text::get_pixel_size() const
		{
			return m_pixel_size;
		}

		const bool Text::is_striked_out() const
		{
			return m_strike_out;
		}

		std::string Text::get_text() const
		{
			return m_text;
		}

		const bool Text::is_underlined() const
		{
			return m_underline;
		}

		std::string Text::get_v_align() const
		{
			return m_valign;
		}

		const bool Text::is_wrapped() const
		{
			return m_wrap;
		}
	} // namespace map
} // namespace galaxy