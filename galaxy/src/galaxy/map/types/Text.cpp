///
/// Text.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/map/MapUtils.hpp"

#include "Text.hpp"

namespace galaxy
{
	namespace map
	{
		Text::Text() noexcept
			: m_bold {false}
			, m_color {255, 255, 255, 255}
			, m_font_family {"sans-serif"}
			, m_halign {"left"}
			, m_italic {false}
			, m_kerning {true}
			, m_pixel_size {16}
			, m_strike_out {false}
			, m_text {""}
			, m_underline {false}
			, m_valign {"top"}
			, m_wrap {false}
		{
		}

		Text::Text(const nlohmann::json& json)
			: m_bold {false}
			, m_color {255, 255, 255, 255}
			, m_font_family {"sans-serif"}
			, m_halign {"left"}
			, m_italic {false}
			, m_kerning {true}
			, m_pixel_size {16}
			, m_strike_out {false}
			, m_text {""}
			, m_underline {false}
			, m_valign {"top"}
			, m_wrap {false}
		{
			parse(json);
		}

		void Text::parse(const nlohmann::json& json)
		{
			if (json.count("bold") > 0)
			{
				m_bold = json.at("bold");
			}

			if (json.count("color") > 0)
			{
				m_color = map::parse_hex_colour(json.at("color"));
			}

			if (json.count("fontfamily") > 0)
			{
				m_font_family = json.at("fontfamily");
			}

			if (json.count("halign") > 0)
			{
				m_halign = json.at("halign");
			}

			if (json.count("italic") > 0)
			{
				m_italic = json.at("italic");
			}

			if (json.count("kerning") > 0)
			{
				m_kerning = json.at("kerning");
			}

			if (json.count("pixelsize") > 0)
			{
				m_pixel_size = json.at("pixelsize");
			}

			if (json.count("strikeout") > 0)
			{
				m_strike_out = json.at("strikeout");
			}

			if (json.count("text") > 0)
			{
				m_text = json.at("text");
			}

			if (json.count("underline") > 0)
			{
				m_underline = json.at("underline");
			}

			if (json.count("valign") > 0)
			{
				m_valign = json.at("valign");
			}

			if (json.count("wrap") > 0)
			{
				m_wrap = json.at("wrap");
			}
		}

		const bool Text::is_bold() const noexcept
		{
			return m_bold;
		}

		const graphics::Colour& Text::get_colour() const noexcept
		{
			return m_color;
		}

		const std::string& Text::get_font_family() const noexcept
		{
			return m_font_family;
		}

		const std::string& Text::get_h_align() const noexcept
		{
			return m_halign;
		}

		const bool Text::is_italic() const noexcept
		{
			return m_italic;
		}

		const bool Text::is_using_kerning() const noexcept
		{
			return m_kerning;
		}

		const int Text::get_pixel_size() const noexcept
		{
			return m_pixel_size;
		}

		const bool Text::is_striked_out() const noexcept
		{
			return m_strike_out;
		}

		const std::string& Text::get_text() const noexcept
		{
			return m_text;
		}

		const bool Text::is_underlined() const noexcept
		{
			return m_underline;
		}

		const std::string& Text::get_v_align() const noexcept
		{
			return m_valign;
		}

		const bool Text::is_wrapped() const noexcept
		{
			return m_wrap;
		}
	} // namespace map
} // namespace galaxy