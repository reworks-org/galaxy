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
		/// A font with a texture map for characters in the font.
		///
		class Font final
		{
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
			/// \return True if successful.
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

		private:
			///
			/// Font atlas.
			///
			Texture m_texture;

			int m_size;
			int m_width;
			int m_height;
			std::string m_name;

			double m_line_height;
			double m_ascender;
			double m_descender;
			double m_underline;
			double m_underline_thickness;

			robin_hood::unordered_flat_map<std::string, msdf_atlas::GlyphGeometry> m_glyphs;
			robin_hood::unordered_flat_map<std::pair<std::string, std::string>, int> m_kerning_pairs;
		};
	} // namespace graphics
} // namespace galaxy

#endif