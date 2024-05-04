///
/// Text.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_TEXT_HPP_
#define GALAXY_GRAPHICS_TEXT_TEXT_HPP_

#include "galaxy/graphics/gl/VertexArray.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/text/Font.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// String of glyphs rendered with a font.
		///
		class Text final
		{
		  public:
			///
			/// Text alignment.
			///
			enum class Alignment : int
			{
				LEFT   = 0,
				CENTER = 1,
				RIGHT  = 2
			};

			///
			/// Constructor.
			///
			Text();

			///
			/// Move constructor.
			///
			Text(Text&&);

			///
			/// Move assignment operator.
			///
			Text& operator=(Text&&);

			///
			/// Destructor.
			///
			~Text();

			///
			/// Creates the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param font Font to use.
			/// \param colour Font colour.
			/// \param alignment Text alignment. Defaults to LEFT.
			///
			void
			create(std::string_view text, const float size, const std::string& font, const graphics::Colour& colour, Alignment alignment = Alignment::LEFT);

			///
			/// Update the font used.
			///
			/// \param font Font to use.
			///
			void set_font(std::string_view font);

			///
			/// Update the rendered text.
			///
			/// \param text Text to display. Supports newlines.
			///
			void update(std::string_view text);

			///
			/// Update the rendered text.
			///
			/// \param size Size of text in em.
			///
			void update(const float size);

			///
			/// Update the rendered text.
			///
			/// \param alignment Text alignment.
			///
			void update(const Alignment alignment);

			///
			/// \brief Get text width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float width() const;

			///
			/// \brief Get text height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float height() const;

			///
			/// \brief Get text.
			///
			/// Is cached for performance.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_text() const;

			///
			/// \brief Get text em size.
			///
			/// Is cached for performance.
			///
			/// \return Float as EM.
			///
			[[nodiscard]] float get_size() const;

			///
			/// Get the text alignment.
			///
			/// \return Alignment enum.
			///
			[[nodiscard]] Alignment get_alignment() const;

			///
			/// Get font ID.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_font() const;

			///
			/// Get vertex array.
			///
			/// \return Reference.
			///
			graphics::VertexArray& vao();

			///
			/// Get render texture.
			///
			/// \return Reference.
			///
			graphics::RenderTexture& render_texture();

		  private:
			///
			/// Copy constructor.
			///
			Text(const Text&) = delete;

			///
			/// Copy assignment operator.
			///
			Text& operator=(const Text&) = delete;

			///
			/// Update the rendered text.
			///
			void update();

		  public:
			///
			/// Text colour.
			///
			graphics::Colour m_colour;

		  private:
			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Font render texture.
			///
			graphics::RenderTexture m_rt;

			///
			/// Font Name.
			///
			std::string m_font_name;

			///
			/// Pointer to font resource.
			///
			graphics::Font* m_font;

			///
			/// Text.
			///
			std::string m_text;

			///
			/// EM size.
			///
			float m_size;

			///
			/// Alignment.
			///
			Alignment m_alignment;
		};
	} // namespace graphics
} // namespace galaxy

#endif
