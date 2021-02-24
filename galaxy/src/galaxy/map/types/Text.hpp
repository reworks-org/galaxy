///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_TEXT_HPP_
#define GALAXY_MAP_TYPES_TEXT_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Data structure on a Text type object.
		///
		class Text final
		{
		public:
			///
			/// Constructor.
			///
			Text() noexcept;

			///
			/// Parse constructor.
			///
			/// \param json JSON structure/array containing ObjectLayer->Object->Text.
			///
			explicit Text(const nlohmann::json& json);

			///
			/// Default destructor.
			///
			~Text() noexcept = default;

			///
			/// Parse text level json.
			///
			/// \param json JSON structure/array containing ObjectLayer->Object->Text.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get bold flag.
			///
			/// \return True if text is bolded.
			///
			[[nodiscard]] const bool is_bold() const noexcept;

			///
			/// Get colour of text.
			///
			/// \return Const reference to graphics::Colour object.
			///
			[[nodiscard]] const graphics::Colour& get_colour() const noexcept;

			///
			/// Get font family.
			///
			/// \return Font type in format: sans-serif (for example).
			///
			[[nodiscard]] const std::string& get_font_family() const noexcept;

			///
			/// Get horizontal alignment value.
			///
			/// \return String in format: center, right, justify or left.
			///
			[[nodiscard]] const std::string& get_h_align() const noexcept;

			///
			/// Get italics flag.
			///
			/// \return True if text is italic.
			///
			[[nodiscard]] const bool is_italic() const noexcept;

			///
			/// Gets kerning flag.
			///
			/// \return True if using kerning to place characters.
			///
			[[nodiscard]] const bool is_using_kerning() const noexcept;

			///
			/// Get pixel (font) size of text.
			///
			/// \return Const int. Pixel size - you will need to convert to font point size.
			///
			[[nodiscard]] const int get_pixel_size() const noexcept;

			///
			/// Get strikeout flag.
			///
			/// \return True if text has been struck out.
			///
			[[nodiscard]] const bool is_striked_out() const noexcept;

			///
			/// Get the text.
			///
			/// \return Actual text as a string.
			///
			[[nodiscard]] const std::string& get_text() const noexcept;

			///
			/// Get underline flag.
			///
			/// \return True if text is underlined.
			///
			[[nodiscard]] const bool is_underlined() const noexcept;

			///
			/// Get vertical alignment.
			///
			/// \return String in format: center, bottom or top.
			///
			[[nodiscard]] const std::string& get_v_align() const noexcept;

			///
			/// Get wrapped flag.
			///
			/// \return True if text is wrapped within object bounds.
			///
			[[nodiscard]] const bool is_wrapped() const noexcept;

		private:
			///
			/// Whether to use a bold font.
			///
			bool m_bold;

			///
			/// Colour object.
			///
			graphics::Colour m_color;

			///
			/// Font family.
			///
			std::string m_font_family;

			///
			/// Horizontal alignment.
			///
			std::string m_halign;

			///
			/// Whether to use an italic font.
			///
			bool m_italic;

			///
			/// Whether to use kerning when placing characters.
			///
			bool m_kerning;

			///
			/// Pixel size of font.
			///
			int m_pixel_size;

			///
			/// Whether to strike out the text.
			///
			bool m_strike_out;

			///
			/// Text.
			///
			std::string m_text;

			///
			/// Whether to underline the text.
			///
			bool m_underline;

			///
			/// Vertical alignment.
			///
			std::string m_valign;

			///
			/// Whether the text is wrapped within the object bounds.
			///
			bool m_wrap;
		};
	} // namespace map
} // namespace galaxy

#endif