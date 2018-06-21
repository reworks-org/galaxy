///
/// TextureAtlas.hpp
/// starlight
///
/// Created by reworks on 29/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TEXTUREATLAS_HPP_
#define STARLIGHT_TEXTUREATLAS_HPP_

#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>

#include "sl/math/Rect.hpp"
#include "sl/types/ResourceCache.hpp"
#include "sl/libs/MaxRectsBinPack/MaxRectsBinPack.hpp"

namespace sl
{
	///
	/// A texture atlas containing all the textures and rasterized text in the game.
	/// This means rendering can be very efficient because only one texture has to be loaded and no switching takes place.
	///
	class TextureAtlas final : public ResourceCache<Rect<int>>
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Loads all textures and performs a maxrectbinpack algorithm on them, then frees textures.
		/// Textures should be located inside textureFolderPath.
		/// 
		/// \param textureFolderPath The location in the VFS where all the textures are stored.
		/// \param powerOfTwoDimension The power of two to create the sprite sheet. E.g. 11 would result in an atas size of 2048x2048.
		///
		TextureAtlas(const std::string& textureFolderPath, int powerOfTwoDimension);

		///
		/// Destructor. Frees texture atlas.
		///
		~TextureAtlas() override;

		///
		/// \brief Add a texture to the atlas.
		///
		/// Usually used when you generate a texture, such as a tilemap, that is not included in the assets.
		///
		/// \param id ID of texture to add. Do not include extension.
		/// \param textureData Bitmap to add. WARNING! textureData WILL NOT BE FREED BY THIS FUNCTION!
		///
		void addTexture(const std::string& id, ALLEGRO_BITMAP* textureData);

		///
		/// \brief Add bitmap text to the atlas.
		///
		/// Usually used when you want to render text.
		///
		/// \param id ID of texture to add. Do not include extension.
		/// \param text Text to draw.
		/// \param font Font to use.
		/// \param col Colour to use.
		///
		void addText(const std::string& id, const std::string& text, ALLEGRO_FONT* font, ALLEGRO_COLOR col);

		///
		/// \brief A function that takes a lua script to add a batch of text.
		///
		/// Example:
		/// TextList =
		/// {
		///     helloWorld =
		///     {
		///         text = "Hello, World!",
		///			font = "ExampleFont",
		///			col = 
		///			{
		///				r = 0,
		///				g = 0,
		///				b = 0,
		///				a = 255
		///			}
		///     }
		/// }
		///
		/// \param script Name of the script file.
		///
		void batchAddText(const std::string& script);

		///
		/// Add a new rectangle to the atlas, but it uses an existing texture.
		///
		/// \param id ID of texture to add. Do not include extension.
		/// \param rect x,y -> Upper Left, Upper Right on atlas, w,h -> Width and Height of area to cover.
		///
		void addRectToAtlas(const std::string& id, const Rect<int>& rect);

		///
		/// Like al_draw_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_bitmap
		///
		void al_draw_packed_bitmap(const std::string& texture, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_bitmap
		///
		void al_draw_tinted_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_scaled_rotated_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_scaled_rotated_bitmap
		///
		void al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags);

		///
		/// 
		/// Calls al_create_sub_bitmap() properly and returns the bitmap of the
		/// packed image. You can then draw and manipulate the bitmap yourself.
		///
		/// Be aware that you need to manually destroy the created bitmap
		/// when you are finished with al_destroy_bitmap()
		///
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP* al_create_packed_sub_bitmap(const std::string& texture);

		///
		/// Draws the internal full texture atlas to the screen. Useful for debugging.
		///
		/// \param x X coord.
		/// \param y Y coord.
		///
		void drawInternalTexture(float x = 0.0f, float y = 0.0f);

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		TextureAtlas() = delete;

		///
		/// Clean up resources.
		/// There is no way to re-create the atlas without destroying it.
		///
		void clean() override;

	private:
		///
		/// The bitmap of all the textures in the game.
		///
		ALLEGRO_BITMAP* m_atlas;

		///
		/// Contains the rectangles outlining all the textures on the atlas bitmap.
		///
		rbp::MaxRectsBinPack m_bin;
	};
}

#endif