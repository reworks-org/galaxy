///
/// Font.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FONT_HPP_
#define QUASAR_FONT_HPP_

#include <string>
#include <unordered_map>

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
		/// Default destructor.
		///
		~Font() noexcept = default;

		///
		/// Load and prep font and characters.
		///
		/// \param file Font file to load.
		/// \param size Size to set the font at.
		///
		void load(const std::string& file, const int size) noexcept;

		///
		/// Create the font.
		///
		/// \param renderer Renderer to draw characters with.
		/// \paramn shader Shader to use when drawing.
		///
		void create(qs::Renderer& renderer, qs::Shader& shader) noexcept;

		///
		/// Retrieve width of a string of text.
		///
		/// \param text Text to get width of.
		///
		const int getTextWidth(const std::string& text) noexcept;
		
		///
		/// Retrieve height of the font.
		///
		const int getHeight() noexcept;
		
		///
		/// Get texture.
		///
		/// \return Pointer to texture atlas.
		///
		qs::BaseTexture* getTexture() noexcept;

		///
		/// Get characters.
		///
		/// \return Returns reference to an unordered_map. Char is key, value is qs::Character.
		///
		std::unordered_map<char, qs::Character>& getChars() noexcept;

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
		/// Stores mapped characters.
		///
		std::unordered_map<char, qs::Character> m_characterMap;
	};
}

#endif