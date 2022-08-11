///
/// Font.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FONT_HPP_
#define GALAXY_GRAPHICS_FONT_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A font is a MSDF atlas of glyphs.
		///
		class Font final
		{
			using GlyphStorage = robin_hood::unordered_flat_map<std::string, msdf_atlas::GlyphGeometry>;
			using Kerning      = std::map<std::pair<std::string, std::string>, int>;

		public:
			///
			/// Constructor.
			///
			Font() noexcept;

			///
			/// Argument constructor.
			///
			/// \param file Path to the font file.
			/// \param size Font size.
			///
			Font(std::string_view file, const int size);

			///
			/// Move constructor.
			///
			Font(Font&&) noexcept;

			///
			/// Move assignment operator.
			///
			Font& operator=(Font&&) noexcept;

			///
			/// Destructor.
			///
			~Font() noexcept;

			///
			/// Creates the font and sets up characters.
			///
			/// \param file Path to the font file.
			/// \param size Font size.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool create(std::string_view file, const int size);

			///
			/// Get font glyph data.
			///
			/// \return Const reference.
			///
			const GlyphStorage& glyphs() const noexcept;

			///
			/// Get font kerning data.
			///
			/// \return Const reference.
			///
			const Kerning& kerning() const noexcept;

		private:
			///
			/// Font atlas.
			///
			Texture m_texture;

			///
			/// Font size.
			///
			int m_size;

			///
			/// Atlas width.
			///
			int m_width;

			///
			/// Atlas height.
			///
			int m_height;

			///
			/// Font name.
			///
			std::string m_name;

			///
			/// Font line height.
			///
			double m_line_height;

			///
			/// Font glyph ascender.
			///
			double m_ascender;

			///
			/// Font glyph descender.
			///
			double m_descender;

			///
			/// Font underlining.
			///
			double m_underline;

			///
			/// Font underline thickness.
			///
			double m_underline_thickness;

			///
			/// Glyph mapping.
			///
			GlyphStorage m_glyphs;

			///
			/// Glyph kerning mapping.
			///
			Kerning m_kerning;
		};
	} // namespace graphics
} // namespace galaxy

#endif