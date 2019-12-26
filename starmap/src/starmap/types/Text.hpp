///
/// Text.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TEXT_HPP_
#define STARMAP_TEXT_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Data structure on a Text type object.
	///
	class Text final
	{
	public:
		///
		/// Default constructor.
		///
		Text() noexcept;

		///
		/// Parse constructor.
		///
		/// \param json JSON structure/array containing ObjectLayer->Object->Text.
		///
		explicit Text(const nlohmann::json& json) noexcept;

		///
		/// Default destructor.
		///
		~Text() noexcept = default;

		///
		/// \brief Parse text level json.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing ObjectLayer->Object->Text.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get bold flag.
		///
		/// \return True if text is bolded.
		///
		const bool isBold() const noexcept;

		///
		/// Get colour as hex string.
		///
		/// \return String in format: #RRGGBB or #AARRGGBB
		///
		const std::string& getColour() const noexcept;

		///
		/// Get font family.
		///
		/// \return Font type in format: sans-serif (for example).
		///
		const std::string& getFontFamily() const noexcept;

		///
		/// Get horizontal alignment value.
		///
		/// \return String in format: center, right, justify or left.
		///
		const std::string& getHAlign() const noexcept;

		///
		/// Get italics flag.
		///
		/// \return True if text is italic.
		///
		const bool isItalic() const noexcept;

		///
		/// Gets kerning flag.
		///
		/// \return True if using kerning to place characters.
		///
		const bool isUsingKerning() const noexcept;

		///
		/// Get pixel (font) size of text.
		///
		/// \return Const int. Pixel size - you will need to convert to font point size.
		///
		const int getPixelSize() const noexcept;

		///
		/// Get strikeout flag.
		///
		/// \return True if text has been struck out.
		///
		const bool isStrikedOut() const noexcept;

		///
		/// Get the text.
		///
		/// \return Actual text as a string.
		///
		const std::string& getText() const noexcept;

		///
		/// Get underline flag.
		///
		/// \return True if text is underlined.
		///
		const bool isUnderlined() const noexcept;

		///
		/// Get vertical alignment.
		///
		/// \return String in format: center, bottom or top.
		///
		const std::string& getVAlign() const noexcept;

		///
		/// Get wrapped flag.
		///
		/// \return True if text is wrapped within object bounds.
		///
		const bool isWrapped() const noexcept;

	private:
		///
		/// Whether to use a bold font.
		///
		bool m_bold;

		///
		/// Hex-formatted color (#RRGGBB or #AARRGGBB) (default: #000000).
		///
		std::string m_color;

		///
		/// Font family.
		///
		std::string m_fontFamily;
		
		///
		/// Horizontal alignment.
		///
		std::string m_hAlign;

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
		int m_pixelSize;

		///
		/// Whether to strike out the text.
		///
		bool m_strikeOut;

		///
		/// Text.
		///
		std::string m_text;

		///
		/// Whether to underline the text.
		///
		bool m_underLine;

		///
		/// Vertical alignment.
		///
		std::string m_vAlign;

		///
		/// Whether the text is wrapped within the object bounds.
		///
		bool m_wrap;
	};
}

#endif