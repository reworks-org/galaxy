///
/// Text.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TEXT_HPP_
#define STARMAP_TEXT_HPP_

#include "nlohmann/json_fwd.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Text-based object.
	///
	class Text
	{
	public:
		///
		/// Default constructor.
		///
		Text() noexcept;

		///
		/// Parse constructor.
		///
		explicit Text(const nlohmann::json& json) noexcept;

		///
		/// Copy constructor.
		///
		Text(const Text&) noexcept = default;

		///
		/// Move constructor.
		///
		Text(Text&&) noexcept = default;

		///
		/// Default destructor.
		///
		~Text() noexcept = default;

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