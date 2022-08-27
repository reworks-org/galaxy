///
/// Font.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FONT_HPP_
#define GALAXY_GRAPHICS_FONT_HPP_

#include <string>

#include <glm/vec2.hpp>

#include "galaxy/graphics/FontContext.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A font is a MSDF atlas of glyphs.
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
			///
			Font(const std::string& file);

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
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool create(const std::string& file);

			///
			/// Get vertical advance for drawing multiline text.
			///
			/// \param size Font size in em.
			///
			/// \return Float.
			///
			[[nodiscard]] float vertical_advance(const float size) const noexcept;

			///
			/// Query text width with font.
			///
			/// \param text Text to query.
			/// \param size Font size in em.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_text_width(const std::string& text, const float size) noexcept;

			///
			/// Query text height with font.
			///
			/// \param text Text to query.
			/// \param size Font size in em.
			///
			/// \return Float.
			///
			[[nodiscard]] float get_text_height(const std::string& text, const float size) noexcept;

			///
			/// Query text size with font.
			///
			/// \param text Text to query.
			/// \param size Font size in em.
			///
			/// \return Vector 2 of dimensions of text.
			///
			[[nodiscard]] glm::vec2 get_text_size(const std::string& text, const float size) noexcept;

		private:
			///
			/// Font object.
			///
			msdfgl_font_t m_font;
		};
	} // namespace graphics
} // namespace galaxy

#endif