///
///  TextureAtlas.hpp
///  rework
///
///  Created by reworks on 29/11/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_TEXTUREATLAS_HPP_
#define REWORK_TEXTUREATLAS_HPP_

#include <unordered_map>
#include <allegro5/color.h>
#include <allegro5/bitmap.h>

#include "re/math/Rect.hpp"
#include "re/types/ServiceLocator.hpp"

namespace re
{
	class TextureAtlas final : public ServiceLocator<TextureAtlas>
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
		/// Like al_draw_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_bitmap
		///
		void al_draw_packed_bitmap(std::string_view texture, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_bitmap
		///
		void al_draw_tinted_packed_bitmap(std::string_view texture, ALLEGRO_COLOR tint, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_scaled_rotated_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_scaled_rotated_bitmap
		///
		void al_draw_tinted_scaled_rotated_packed_bitmap(std::string_view texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags);

		///
		/// Calls al_create_sub_bitmap() properly and returns the bitmap of the
		/// packed image. You can then draw and manipulate the bitmap yourself.
		///
		/// Be aware that you need to manually destroy the created bitmap
		/// when you are finished with al_destroy_bitmap()
		///
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP* al_create_packed_bitmap(std::string_view texture);

	private:
		ALLEGRO_BITMAP* m_atlas;
		std::unordered_map<std::string_view, Rect<int>> m_packedTextures;

	private:
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