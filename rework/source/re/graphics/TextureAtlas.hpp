//
//  TextureAtlas.hpp
//  rework
//
//  Created by reworks on 29/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TEXTUREATLAS_HPP_
#define REWORK_TEXTUREATLAS_HPP_

#include <unordered_map>

#include <allegro5/color.h>
#include <allegro5/bitmap.h>

#include "re/math/Rect.hpp"
#include "re/types/Service.hpp"

namespace re
{
	class TextureAtlas : public Service
	{
	public:
		///
		/// \brief Cosntructor.
		///
		/// Loads all textures and performs a maxrectbinpack algorithm on them, then frees textures.
		///
		TextureAtlas();

		///
		/// Destructor. Frees texture atlas.
		///
		~TextureAtlas() override;

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
		/// Calls al_create_sub_bitmap() properly and returns the bitmap of the
		/// packed image. You can then draw and manipulate the bitmap yourself.
		///
		/// Be aware that you need to manually destroy the created bitmap
		/// when you are finished with al_destroy_bitmap()
		///
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP* al_create_packed_bitmap(const std::string& texture);

	private:
		ALLEGRO_BITMAP* m_atlas;
		size_t m_powerOfTwoDimension;
		std::unordered_map<std::string, Rect<int>> m_packedTextures;
	};
}

#endif