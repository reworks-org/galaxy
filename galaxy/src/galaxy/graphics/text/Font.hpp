///
/// Font.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXT_FONT_HPP_
#define GALAXY_GRAPHICS_TEXT_FONT_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/text/Character.hpp"
#include "galaxy/graphics/texture/RenderTexture.hpp"
#include "galaxy/graphics/Shader.hpp"

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
			Font();

			///
			/// Argument constructor.
			///
			/// \param filepath Path to the font file.
			/// \param size Font size.
			///
			/// \return True if successful.
			///
			Font(std::string_view filepath, const int size);

			///
			/// Copy constructor.
			///
			Font(const Font&) = delete;

			///
			/// Copy assignment operator.
			///
			Font& operator=(const Font&) = delete;

			///
			/// Destructor.
			///
			~Font();

			///
			/// Creates the font and sets up characters.
			///
			/// \param filepath Path to the font file.
			/// \param size Font size.
			///
			/// \return True if successful.
			///
			bool create(std::string_view filepath, const int size);

			///
			/// Get a character.
			///
			/// \param c Retrieve a character from the map.
			///
			/// \return Pointer to character object.
			///
			[[nodiscard]] Character* get_char(char c);

			///
			/// Get fontmap.
			///
			/// \return Pointer to fontmap.
			///
			[[nodiscard]] RenderTexture* get_fontmap();

			///
			/// Retrieve width of a string of text.
			///
			/// \param text Text to get width of.
			///
			[[nodiscard]] const int get_width(std::string_view text);

			///
			/// Retrieve font height.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_height() const;

		private:
			///
			/// Get the height of the font.
			///
			int m_height;

			///
			/// Character map to lookup characters.
			///
			robin_hood::unordered_map<char, Character> m_characters;

			///
			/// Render Texture.
			///
			RenderTexture m_fontmap;

			///
			/// Shader to draw font with.
			///
			Shader m_shader;
		};
	} // namespace graphics
} // namespace galaxy

#endif