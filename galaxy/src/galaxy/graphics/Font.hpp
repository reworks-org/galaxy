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
			/// Loads the font and sets up characters.
			///
			/// \param file Path to the font file.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool load(const std::string& file);

			///
			/// \brief Build the font atlas.
			///
			/// Not thread safe, calls OpenGL code.
			///
			void build();

			///
			/// Get vertical advance for drawing multiline text.
			///
			/// \param size Font size in em.
			///
			/// \return Float.
			///
			[[nodiscard]] float vertical_advance(const float size) const noexcept;

			///
			/// \brief Query text size with font.
			///
			/// Doesnt support vertical text. You also need to pre-process tab symbols.
			///
			/// \param text Text to query.
			/// \param size Font size in em.
			///
			/// \return Vector 2 of dimensions of text.
			///
			[[nodiscard]] glm::vec2 get_text_size(const std::string& text, const float size) noexcept;

			///
			/// Get handle to internal font object.
			///
			/// \return msdfgl_font_t pointer.
			///
			[[nodiscard]] msdfgl_font_t handle() noexcept;

		private:
			///
			/// Font object.
			///
			msdfgl_font_t m_font;

			///
			/// \brief Freetype face.
			///
			/// Owned by MSDFGL.
			///
			FT_Face m_face;
		};
	} // namespace graphics
} // namespace galaxy

#endif