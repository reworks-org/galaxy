///
/// Font.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FONT_HPP_
#define QUASAR_FONT_HPP_

#include <robin_hood.h>

#include "qs/text/FreeType.hpp"
#include "qs/text/Character.hpp"
#include "qs/texture/RenderTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward decs.
	///
	class Shader;
	class Renderer;

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
		/// Load constructor.
		///
		/// \param file Font file to load.
		/// \param size Size to set the font at.
		///
		Font(std::string_view file, const unsigned int size);

		///
		/// Copy constructor.
		///
		Font(const Font&) noexcept = delete;

		///
		/// Move constructor.
		///
		Font(Font&&);

		///
		/// Copy assignment operator.
		///
		Font& operator=(const Font&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Font& operator=(Font&&);

		///
		/// Default destructor.
		///
		~Font() noexcept = default;

		///
		/// Load and prep font and characters.
		///
		/// \param file Font file to load.
		/// \param size Size to set the font at.
		///
		void load(std::string_view file, const unsigned int size);

		///
		/// Create the font.
		///
		/// \param renderer Renderer to draw characters with.
		/// \paramn shader Shader to use when drawing.
		///
		void create(qs::Renderer& renderer, qs::Shader& shader);

		///
		/// Retrieve width of a string of text.
		///
		/// \param text Text to get width of.
		///
		[[nodiscard]] const int get_text_width(const std::string& text) noexcept;

		///
		/// Retrieve height of the font.
		///
		[[nodiscard]] const int get_height() noexcept;

		///
		/// Get texture.
		///
		/// \return Pointer to texture atlas.
		///
		[[nodiscard]] qs::BaseTexture* get_texture() noexcept;

		///
		/// Get a character.
		///
		/// \return Returns pointer to Character class.
		///
		[[nodiscard]] qs::Character* get_char(const char c) noexcept;

	private:
		///
		/// Cached height of font.
		///
		int m_height;

		///
		/// Texture.
		///
		qs::RenderTexture m_texture;

		///
		/// Character map.
		///
		robin_hood::unordered_map<char, qs::Character> m_characters;
	};
} // namespace qs

#endif