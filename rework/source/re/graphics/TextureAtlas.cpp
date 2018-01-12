///
///  TextureAtlas.cpp
///  rework
///
///  Created by reworks on 29/11/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include <physfs.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_font.h>

#include "loguru/loguru.hpp"
#include "re/utils/Utils.hpp"
#include "re/graphics/Window.hpp"

#include "TextureAtlas.hpp"

namespace re
{
	TextureAtlas::TextureAtlas(size_t powerOfTwoDimension)
	{
		size_t atlasSize = std::pow(2, powerOfTwoDimension);
		m_atlas = al_create_bitmap(atlasSize, atlasSize);
		m_bin.Init(atlasSize, atlasSize, false);

		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		al_set_target_bitmap(m_atlas);
		al_clear_to_color(al_map_rgba(255, 255, 255, 255));
		
		char** efl = PHYSFS_enumerateFiles("textures");
		for (char** i = efl; *i != NULL; i++)
		{
			ALLEGRO_BITMAP* bitmap = al_load_bitmap(*i);
			Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), heuristic);

			if (!(packedRect.m_height > 0))
			{
				LOG_S(WARNING) << "Failed to pack a texture! Texture: " << *i;
			}

			al_draw_bitmap(bitmap, packedRect.m_x, packedRect.m_y, 0);
			m_resourceMap.emplace(entt::HashedString(utils::removeExtension(std::string(*i)).c_str()), packedRect);
			
			al_destroy_bitmap(bitmap);
		}

		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Window::get()->getDisplay()));
		
		PHYSFS_freeList(efl);
	}

	TextureAtlas::~TextureAtlas()
	{
		clean();
	}

	void TextureAtlas::addTextureToAtlas(entt::HashedString ID, ALLEGRO_BITMAP* textureData)
	{
		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(textureData), al_get_bitmap_height(textureData), heuristic);

		if (!(packedRect.m_height > 0))
		{
			LOG_S(WARNING) << "Failed to pack a texture! Texture: " << ID;
		}

		al_set_target_bitmap(m_atlas);
		al_draw_bitmap(textureData, packedRect.m_x, packedRect.m_y, 0);
		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Window::get()->getDisplay()));

		m_resourceMap.emplace(ID, packedRect);
	}

	void TextureAtlas::addTextToAtlas(entt::HashedString ID, const std::string& text, ALLEGRO_FONT* font, ALLEGRO_COLOR col)
	{
		int w = al_get_text_width(font, text.c_str());
		int h = al_get_font_line_height(font);
		ALLEGRO_BITMAP* bitmap = al_create_bitmap(w, h);
		
		al_set_target_bitmap(bitmap);
		al_clear_to_color(al_map_rgba(255, 255, 255, 255));
		al_draw_text(font, col, 0, 0, 0, text.c_str());
		al_flip_display();

		addTextureToAtlas(ID, bitmap);

		al_destroy_bitmap(bitmap);
	}

	void TextureAtlas::al_draw_packed_bitmap(entt::HashedString texture, float dx, float dy, int flags)
	{
		auto pr = m_resourceMap[texture];
		al_draw_bitmap_region(m_atlas, pr.x, pr.y, pr.width, pr.height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_packed_bitmap(entt::HashedString texture, ALLEGRO_COLOR tint, float dx, float dy, int flags) 
	{
		auto pr = m_resourceMap[texture];
		al_draw_tinted_bitmap_region(m_atlas, tint, pr.x, pr.y, pr.width, pr.height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_scaled_rotated_packed_bitmap(entt::HashedString texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags)
	{
		auto pr = m_resourceMap[texture];
		al_draw_tinted_scaled_rotated_bitmap_region(m_atlas, pr.x, pr.y, pr.width, pr.height, tint, cx, cy, dx, dy, xscale, yscale, angle, flags);
	}

	void TextureAtlas::clean()
	{
		al_destroy_bitmap(m_atlas);
	}

	ALLEGRO_BITMAP* TextureAtlas::al_create_packed_bitmap(entt::HashedString texture)
	{
		auto pr = m_resourceMap[texture];
		return al_create_sub_bitmap(m_atlas, pr.x, pr.y, pr.width, pr.height);
	}
}