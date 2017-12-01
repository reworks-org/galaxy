//
//  TextureAtlas.cpp
//  rework
//
//  Created by reworks on 29/11/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include <physfs.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "loguru/loguru.hpp"
#include "re/utils/Utils.hpp"
#include "re/graphics/Window.hpp"
#include "re/math/MaxRectsBinPack.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TextureAtlas.hpp"

namespace re
{
	TextureAtlas::TextureAtlas(size_t powerOfTwoDimension)
	{
		size_t atlasSize = std::pow(2, powerOfTwoDimension);
		m_atlas = al_create_bitmap(atlasSize, atlasSize);
		rbp::MaxRectsBinPack bin(atlasSize, atlasSize, false);
		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		al_set_target_bitmap(m_atlas);
		al_clear_to_color(al_map_rgba(255, 255, 255, 255));
		
		char** efl = PHYSFS_enumerateFiles("textures");
		for (char** i = efl; *i != NULL; i++)
		{
			ALLEGRO_BITMAP* bitmap = al_load_bitmap(*i);
			Rect<int> packedRect = bin.Insert(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), heuristic);

			if (!(packedRect.height > 0))
			{
				LOG_S(WARNING) << "Failed to pack a texture! Texture: " << *i << std::endl;
			}

			al_draw_bitmap(bitmap, packedRect.x, packedRect.y, 0);
			m_packedTextures.emplace(Utils::removeExtension(std::string(*i)), packedRect);
			
			al_destroy_bitmap(bitmap);
		}

		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Locator::get<Window>()->getDisplay()));
		
		PHYSFS_freeList(efl);
	}

	TextureAtlas::~TextureAtlas()
	{
		al_destroy_bitmap(m_atlas);
	}

	void TextureAtlas::al_draw_packed_bitmap(const std::string& texture, float dx, float dy, int flags)
	{
		auto pr = m_packedTextures[texture];
		al_draw_bitmap_region(m_atlas, pr.x, pr.y, pr.width, pr.height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float dx, float dy, int flags) 
	{
		auto pr = m_packedTextures[texture];
		al_draw_tinted_bitmap_region(m_atlas, tint, pr.x, pr.y, pr.width, pr.height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags)
	{
		auto pr = m_packedTextures[texture];
		al_draw_tinted_scaled_rotated_bitmap_region(m_atlas, pr.x, pr.y, pr.width, pr.height, tint, cx, cy, dx, dy, xscale, yscale, angle, flags);
	}

	ALLEGRO_BITMAP* TextureAtlas::al_create_packed_bitmap(const std::string& texture)
	{
		auto pr = m_packedTextures[texture];
		return al_create_sub_bitmap(m_atlas, pr.x, pr.y, pr.width, pr.height);
	}
}