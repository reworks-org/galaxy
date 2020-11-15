///
/// Font.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FONT_HPP_
#define QUASAR_FONT_HPP_

#include <robin_hood.h>

#include "qs/core/Shader.hpp"
#include "qs/text/Character.hpp"
#include "qs/texture/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
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
		/// Move constructor.
		///
		Font(Font&&) = default;

		///
		/// Copy assignment operator.
		///
		Font& operator=(const Font&) = delete;

		///
		/// Move assignment operator.
		///
		Font& operator=(Font&&) = default;

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
		[[nodiscard]] qs::Character* get_char(char c);

		///
		/// Get fontmap.
		///
		/// \return Pointer to fontmap.
		///
		[[nodiscard]] qs::RenderTexture* get_fontmap();

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
		robin_hood::unordered_map<char, qs::Character> m_characters;

		///
		/// Render Texture.
		///
		qs::RenderTexture m_fontmap;

		///
		/// Shader to draw font with.
		///
		qs::Shader m_shader;
	};
} // namespace qs

#endif