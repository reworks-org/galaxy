///
///  TextureAtlas.hpp
///  starlight
///
///  Created by reworks on 29/11/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_TEXTUREATLAS_HPP_
#define STARLIGHT_TEXTUREATLAS_HPP_

#include <unordered_map>
#include <allegro5/color.h>
#include <allegro5/bitmap.h>
#include <allegro5/allegro_font.h>

#include "sl/math/Rect.hpp"
#include "sl/types/ResourceCache.hpp"
#include "sl/math/MaxRectsBinPack.hpp"
#include "sl/types/ServiceLocator.hpp"

namespace sl
{
	class TextureAtlas final : public ServiceLocator<TextureAtlas>, public ResourceCache<Rect<int>>
	{
	public:
		///
		/// \brief Constructor.
		///
		/// Loads all textures and performs a maxrectbinpack algorithm on them, then frees textures.
		///
		/// \param powerOfTwoDimension The power of two to create the sprite sheet. E.g. 11 would result in an atas size of 2048x2048.
		///
		TextureAtlas(size_t powerOfTwoDimension);

		///
		/// Destructor. Frees texture atlas.
		///
		~TextureAtlas() override;

		///
		/// \brief Add a texture to the atlas.
		///
		/// Usually used when you generate a texture, such as a tilemap, that is not included in the assets.
		///
		/// \param ID ID of texture to add. Do not include extension.
		/// \param textureData Bitmap to add. WARNING! textureData WILL NOT BE FREED BY THIS FUNCTION!
		///
		void addTextureToAtlas(entt::HashedString ID, ALLEGRO_BITMAP* textureData);

		///
		/// \brief Add bitmap text to the atlas.
		///
		/// Usually used when you want to render text.
		///
		/// \param ID ID of texture to add. Do not include extension.
		/// \param text Text to draw.
		/// \param font Font to use.
		/// \param col Colour to use.
		///
		void addTextToAtlas(entt::HashedString ID, const std::string& text, ALLEGRO_FONT* font, ALLEGRO_COLOR col);

		///
		/// Like al_draw_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_bitmap
		///
		void al_draw_packed_bitmap(entt::HashedString, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_bitmap
		///
		void al_draw_tinted_packed_bitmap(entt::HashedString texture, ALLEGRO_COLOR tint, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_scaled_rotated_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_scaled_rotated_bitmap
		///
		void al_draw_tinted_scaled_rotated_packed_bitmap(entt::HashedString texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags);

		///
		/// Calls al_create_sub_bitmap() properly and returns the bitmap of the
		/// packed image. You can then draw and manipulate the bitmap yourself.
		///
		/// Be aware that you need to manually destroy the created bitmap
		/// when you are finished with al_destroy_bitmap()
		///
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP* al_create_packed_bitmap(entt::HashedString texture);

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		ALLEGRO_BITMAP* m_atlas;
		rbp::MaxRectsBinPack m_bin;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		TextureAtlas() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		TextureAtlas(const TextureAtlas&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		TextureAtlas(TextureAtlas&&) = delete;
	};
}

#endif