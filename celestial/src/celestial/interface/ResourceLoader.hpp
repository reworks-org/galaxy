///
/// ResourceLoader.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_RESOURCELOADER_HPP_
#define CELESTIAL_RESOURCELOADER_HPP_

#include <string>

#include <protostar/shapes/Rect.hpp>
#include <protostar/graphics/Colour.hpp>

#include "celestial/interface/Font.hpp"
#include "celestial/interface/Texture.hpp"

///
/// Core namespace.
///	
namespace celestial
{
	///
	/// Identifies this class as an interface.
	///
	namespace interface
	{
		///
		/// Manipulating resources for use in celestial.
		///
		class ResourceLoader
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~ResourceLoader() noexcept = default;

			///
			/// Loads a texture.
			///
			/// \param id ID or location or file of texture to load.
			///
			/// \return Polymorphic texture pointer to the newly created texture.
			///
			virtual TexturePtr loadTexture(const std::string& id) noexcept = 0;

			///
			/// Loads a font.
			///
			/// \param id ID or location or file of font to load.
			///
			/// \return Polymorphic font pointer to the newly created font.
			///
			virtual FontPtr loadFont(const std::string& id) noexcept = 0;

			///
			/// Creates a brand new sectioned texture.
			///
			/// \param texture Texture to take to form a sectioned texture from.
			/// \param xLeft The leftmost part of the texture to start taking from.
			/// \param xRight The rightmost part of the texture to start taking from.
			///
			/// \return Polymorphic texture pointer to the newly created texture.
			///
			virtual TexturePtr createSectionedTexture(celestial::interface::Texture* texture, const int xLeft, const int xRight) noexcept = 0;

			///
			/// Creates a new texture from a region of a master texture.
			///
			/// \param texture Master texture to extract texture region from.
			/// \param dim Dimensions of the region of the texture to use.
			///
			/// \return New polymorphic texture of a region of the master texture.
			///
			virtual TexturePtr createSubTexture(celestial::interface::Texture* id, const protostar::Rect<int>& dim) noexcept = 0;

			///
			/// Creates a geometric coloured rectangle.
			///
			/// \param width Width of the rectangle to create.
			/// \param height Height of the rectangle to create.
			/// \param colour Colour of the rectangle to use.
			///
			/// \return New polymorphic texture.
			///
			virtual TexturePtr createRectangle(const int width, const int height, const protostar::Colour& colour) noexcept = 0;

			///
			/// Retrieves the height of a texture.
			///
			/// \param texture Texture to get the height of.
			///
			/// \return height in integer form.
			///
			virtual int getTextureHeight(const celestial::interface::Texture* texture) noexcept = 0;

			///
			/// Retrieves the width of a texture.
			///
			/// \param texture Texture to get the width of.
			///
			/// \return width in integer form.
			///
			virtual int getTextureWidth(const celestial::interface::Texture* texture) noexcept = 0;

			///
			/// Retrieves the height of a string.
			///
			/// \param font Font of the text.
			/// \param text Text to use.
			///
			/// \return height in integer form.
			///
			virtual int getTextHeight(const celestial::interface::Font* font, const std::string& text) noexcept = 0;

			///
			/// Retrieves the width of a string.
			///
			/// \param font Font of the text.
			/// \param text Text to use.
			///
			/// \return width in integer form.
			///
			virtual int getTextWidth(const celestial::interface::Font* font, const std::string& text) noexcept = 0;

			///
			/// Retrieves the line height of a font.
			///
			/// \param font Font to get line height from.
			///
			/// \return line height in integer.
			///
			virtual int getLineHeight(const celestial::interface::Font* font) noexcept = 0;

		protected:
			///
			/// Constructor.
			///
			ResourceLoader() noexcept = default;
			
			///
			/// Copy constructor.
			///
			ResourceLoader(const ResourceLoader&) noexcept = default;

			///
			/// Move constructor.
			///
			ResourceLoader(ResourceLoader&&) noexcept = default;
		};
	}
}

#endif