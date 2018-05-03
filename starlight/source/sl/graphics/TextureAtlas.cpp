///
/// TextureAtlas.cpp
/// starlight
///
/// Created by reworks on 29/11/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <physfs.h>
#include <allegro5/display.h>
#include <allegro5/drawing.h>
#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "sl/utils/Utils.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "TextureAtlas.hpp"

namespace sl
{
	TextureAtlas::TextureAtlas(int powerOfTwoDimension)
	{
		int atlasSize = std::pow(2, powerOfTwoDimension);
		m_atlas = al_create_bitmap(atlasSize, atlasSize);
		m_bin.Init(atlasSize, atlasSize, false);

		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		al_set_target_bitmap(m_atlas);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));
		
		char** efl = PHYSFS_enumerateFiles("textures");

		if (!efl)
		{
			LOG_S(WARNING) << "Could not find textures!";
		}
		else
		{
			for (char** i = efl; *i != NULL; i++)
			{
				std::string loc = "textures/" + std::string(*i);
				ALLEGRO_BITMAP* bitmap = al_load_bitmap(loc.c_str());
				Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), heuristic);

				if (!(packedRect.m_height > 0))
				{
					LOG_S(WARNING) << "Failed to pack a texture! Texture: " << *i;
				}

				al_draw_bitmap(bitmap, packedRect.m_x, packedRect.m_y, 0);
				m_resourceMap.emplace(entt::HashedString{ utils::removeExtension(*i).c_str() }, packedRect);

				al_destroy_bitmap(bitmap);
			}
		}
		
		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Locator::window->getDisplay()));
		
		if (efl != NULL)
		{
			PHYSFS_freeList(efl);
		}
	}

	TextureAtlas::~TextureAtlas()
	{
		clean();
	}

	void TextureAtlas::addTextureToAtlas(const std::string& ID, ALLEGRO_BITMAP* textureData)
	{
		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(textureData), al_get_bitmap_height(textureData), heuristic);

		if (packedRect.m_height <= 0)
		{
			LOG_S(WARNING) << "Failed to pack a texture! Texture: " << ID;
		}

		al_set_target_bitmap(m_atlas);
		al_draw_bitmap(textureData, packedRect.m_x, packedRect.m_y, 0);
		al_flip_display();
		al_set_target_backbuffer(Locator::window->getDisplay());

		m_resourceMap.emplace(entt::HashedString{ ID.c_str() }, packedRect);
	}

	void TextureAtlas::addTextToAtlas(const std::string& ID, const std::string& text, ALLEGRO_FONT* font, ALLEGRO_COLOR col)
	{
		int w = al_get_text_width(font, text.c_str());
		int h = al_get_font_line_height(font);
		ALLEGRO_BITMAP* bitmap = al_create_bitmap(w, h);
		
		al_set_target_bitmap(bitmap);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));
		al_draw_text(font, col, 0, 0, 0, text.c_str());
		al_flip_display();

		addTextureToAtlas(ID, bitmap);

		al_destroy_bitmap(bitmap);
	}

	void TextureAtlas::addRectToAtlas(const std::string& ID, const Rect<int>& rect)
	{
		m_resourceMap.emplace(entt::HashedString{ ID.c_str() }, rect);
	}

	void TextureAtlas::al_draw_packed_bitmap(const std::string& texture, float dx, float dy, int flags)
	{
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];
		al_draw_bitmap_region(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float dx, float dy, int flags)
	{
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];
		al_draw_tinted_bitmap_region(m_atlas, tint, pr.m_x, pr.m_y, pr.m_width, pr.m_height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags)
	{
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];
		al_draw_tinted_scaled_rotated_bitmap_region(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height, tint, cx, cy, dx, dy, xscale, yscale, angle, flags);
	}

	void TextureAtlas::clean()
	{
		al_destroy_bitmap(m_atlas);
	}

	ALLEGRO_BITMAP* TextureAtlas::al_create_packed_sub_bitmap(const std::string& texture)
	{
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];
		return al_create_sub_bitmap(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height);
	}

	void TextureAtlas::drawInternalTexture(float x, float y)
	{
		al_draw_bitmap(m_atlas, x, y, 0);
	}
}