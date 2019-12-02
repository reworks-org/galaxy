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
		/// Loads a texture. Make sure texture takes care of deleting its own data.
		///
		virtual TexturePtr loadTexture(const std::string& id) = 0;

		///
		///
		///
		virtual FontPtr loadFont(const std::string& id) = 0;

		///
		/// The provided texture will be freed.
		///
		virtual TexturePtr createSectionedTexture(TexturePtr texture, const int xLeft, const int xRight) = 0;

		///
		///
		///
		virtual TexturePtr loadSubBitmap(const std::string& id, const celestial::Rect<int>& dim) = 0;

		///
		///
		///
		virtual TexturePtr createRectangle(const int width, const int height, const celestial::Colour* colour) = 0;

		///
		///
		///
		virtual FontPtr loadFont(const std::string& font) = 0;
		
		///
		///
		///
		virtual int getTextureHeight(const celestial::Texture* texture) = 0;

		///
		///
		///
		virtual int getTextureWidth(const celestial::Texture* texture) = 0;

		///
		///
		///
		virtual int getTextHeight(const std::string& text) = 0;

		///
		///
		///
		virtual int getTextWidth(const std::string& text) = 0;
	};
}

#endif