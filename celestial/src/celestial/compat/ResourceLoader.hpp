///
/// ResourceLoader.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_RESOURCELOADER_HPP_
#define CELESTIAL_RESOURCELOADER_HPP_

namespace celestial
{
	///
	/// Manipulating resources for use in celestial.
	///
	class ResourceLoader
	{
	public:
		///
		/// Loads a texture.
		///
		/// \param id ID or location or file of texture to load.
		///
		/// \return Polymorphic texture pointer to the newly created texture.
		///
		virtual TexturePtr loadTexture(const std::string& id) = 0;

		///
		/// Loads a font.
		///
		/// \param id ID or location or file of font to load.
		///
		/// \return Polymorphic font pointer to the newly created font.
		///
		virtual FontPtr loadFont(const std::string& id) = 0;

		///
		/// Creates a brand new sectioned texture.
		///
		/// \param texture Texture to take to form a sectioned texture from.
		/// \param xLeft The leftmost part of the texture to start taking from.
		/// \param xRight The rightmost part of the texture to start taking from.
		///
		/// \return Polymorphic texture pointer to the newly created texture.
		///
		virtual TexturePtr createSectionedTexture(celestial::Texture* texture, const int xLeft, const int xRight) = 0;

		///
		/// Creates a new texture from a region of a master texture.
		///
		/// \param texture Master texture to extract texture region from.
		/// \param dim Dimensions of the region of the texture to use.
		///
		/// \return New polymorphic texture of a region of the master texture.
		///
		virtual TexturePtr createSubTexture(celestial::Texture* id, const celestial::Rect<int>& dim) = 0;

		///
		/// Creates a geometric coloured rectangle.
		///
		/// \param width Width of the rectangle to create.
		/// \param height Height of the rectangle to create.
		/// \param colour Colour of the rectangle to use.
		///
		/// \return New polymorphic texture.
		///
		virtual TexturePtr createRectangle(const int width, const int height, const celestial::compat::Colour* colour) = 0;

		///
		/// Retrieves the height of a texture.
		///
		/// \param texture Texture to get the height of.
		///
		/// \return height in integer form.
		///
		virtual int getTextureHeight(const celestial::Texture* texture) = 0;

		///
		/// Retrieves the width of a texture.
		///
		/// \param texture Texture to get the width of.
		///
		/// \return width in integer form.
		///
		virtual int getTextureWidth(const celestial::Texture* texture) = 0;

		///
		/// Retrieves the height of a string.
		///
		/// \param font Font of the text.
		/// \param text Text to use.
		///
		/// \return height in integer form.
		///
		virtual int getTextHeight(const celestial::Font* font, const std::string& text) = 0;

		///
		/// Retrieves the width of a string.
		///
		/// \param font Font of the text.
		/// \param text Text to use.
		///
		/// \return width in integer form.
		///
		virtual int getTextWidth(const celestial::Font* font, const std::string& text) = 0;

		///
		/// Retrieves the line height of a font.
		///
		/// \param font Font to get line height from.
		///
		/// \return line height in integer.
		///
		virtual int getLineHeight(const celestial::Font* font) = 0;
	};
}

#endif