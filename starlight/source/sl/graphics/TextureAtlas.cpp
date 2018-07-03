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

#include "sl/fs/VirtualFS.hpp"
#include "sl/math/Vector2.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/resources/FontBook.hpp"
#include "sl/physics/Box2DHelper.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "TextureAtlas.hpp"

namespace sl
{
	TextureAtlas::TextureAtlas(const std::string& textureFolderPath, int powerOfTwoDimension)
	{
		// Create atlas size from power of 2, since the algorithms work best with power of 2's.
		int atlasSize = std::pow(2, powerOfTwoDimension);
		m_atlas = al_create_bitmap(atlasSize, atlasSize);

		// Set the algorithm to use when packing the bin, in this case bestshortsidefit.
		m_bin.Init(atlasSize, atlasSize, false);
		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;

		al_set_target_bitmap(m_atlas);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));
		
		// Next loop over each texture in the folder and add it to a list.
		char** efl = PHYSFS_enumerateFiles(textureFolderPath.c_str());
		if (!efl)
		{
			LOG_S(FATAL) << "Could not find textures or folder was empty! PhysFS Error: " << PHYSFS_getLastError();
		}
		else
		{
			// For each in folder...
			for (char** i = efl; *i != NULL; i++)
			{
				// ...create a bitmap of the texture...
				std::string loc = "textures/" + std::string(*i);
				ALLEGRO_BITMAP* bitmap = al_load_bitmap(loc.c_str());

				/// ...then fit it into the atlas...
				Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), heuristic);
				if (!(packedRect.m_height > 0))
				{
					LOG_S(FATAL) << "Failed to pack a texture! Texture: " << *i;
				}

				// ...then draw it to the atlas bitmap.
				al_draw_bitmap(bitmap, packedRect.m_x, packedRect.m_y, 0);
				m_resourceMap.emplace(entt::HashedString{ Utils::removeExtension(*i).c_str() }, packedRect);

				al_destroy_bitmap(bitmap);
			}
		}
		
		// Clean up, ensuring the display is set correctly.
		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Locator::window->getDisplay()));
		
		// Free up the list.
		PHYSFS_freeList(efl);
	}

	TextureAtlas::~TextureAtlas()
	{
		clean();
	}

	void TextureAtlas::addTexture(const std::string& id, ALLEGRO_BITMAP* textureData)
	{
		// Set the algorithm to use when packing the bin, in this case bestshortsidefit.
		rbp::MaxRectsBinPack::FreeRectChoiceHeuristic heuristic = rbp::MaxRectsBinPack::RectBestShortSideFit;
		int w = al_get_bitmap_width(textureData);
		int h = al_get_bitmap_height(textureData);
		Rect<int> packedRect = m_bin.Insert(w, h, heuristic);
		
		// Correct width and height.
		// For some reason the algorithm was reversing this.
		packedRect.m_width = w;
		packedRect.m_height = h;

		// Making sure it was actually packed.
		if (packedRect.m_height <= 0)
		{
			LOG_S(FATAL) << "Failed to pack a texture! Texture: " << id;
		}

		// Then draw it to the master bitmap by changing the default rendering target to the atlas.
		al_set_target_bitmap(m_atlas);
		al_draw_bitmap(textureData, packedRect.m_x, packedRect.m_y, 0);
		al_flip_display();

		// Then make sure rendering target is set back to window.
		al_set_target_backbuffer(Locator::window->getDisplay());

		m_resourceMap.emplace(entt::HashedString{ id.c_str() }, packedRect);
	}

	void TextureAtlas::addText(const std::string& id, const std::string& text, ALLEGRO_FONT* font, ALLEGRO_COLOR col, int flags)
	{
		// We set as a pair with a vector2 because it lets us pass 2 arguments through extra, along with font.
		// Because allegro wont except a lamba with a capture, and I don't want any global functions or variables.
		// Vector2 default-constructs to 0, which is what is wanted.
		std::pair<ALLEGRO_FONT*, Vector2<int>> fwh;
		fwh.first = font;

		// Calculate bitmap width and height.
		al_do_multiline_text(font, text.length(), text.c_str(), [](int line_num, const char* line, int size, void* extra) -> bool
		{
			// Get fwh pointer.
			auto* fwhPtr = static_cast<std::pair<ALLEGRO_FONT*, Vector2<int>>*>(extra);

			// Retrieve text dimensions.
			int textWidth = 0;
			int textHeight = 0;
			int bbx, bby; // We dont care about this.
			
			al_get_text_dimensions(fwhPtr->first, line, &bbx, &bby, &textWidth, &textHeight);

			// Make sure the width is the same as the largest text line's width.
			if (textWidth >= fwhPtr->second.m_x)
			{
				fwhPtr->second.m_x = textWidth;
			}

			// And that the height takes into account each line of text and the gap between lines.
			fwhPtr->second.m_y += (textHeight + al_get_font_line_height(fwhPtr->first));

			return true;
		}, &fwh);

		ALLEGRO_BITMAP* bitmap = al_create_bitmap(fwh.second.m_x, fwh.second.m_y);

		// Then draw the text to that bitmap so it can be added to the atlas.
		al_set_target_bitmap(bitmap);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));

		// We pass 0 for line height so allegro defaults to font_line_height like in the calculations above.
		al_draw_multiline_text(font, col, 0, 0, text.length(), 0, flags, text.c_str());

		al_flip_display();

		// Then add that bitmap to the atlas.
		addTexture(id.c_str(), bitmap);

		// We only have to destroy bitmap because addTexture() resets rendertarget to display.
		al_destroy_bitmap(bitmap);
	}

	void TextureAtlas::batchAddText(const std::string& script)
	{
		// Set up a lua state to parse script.
		sol::state loader;
		loader.script(Locator::virtualFS->openAsString(script));

		// Iterate over script getting each text configuration.
		sol::table textList = loader.get<sol::table>("TextList");
		textList.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			sol::table data = pair.second.as<sol::table>();
			sol::table col = data.get<sol::table>("col");

			// Then add the text to the atlas using the existing addText method, using the data retrieved from the lua file.
			addText(pair.first.as<std::string>(), data.get<std::string>("text"), Locator::fontBook->get(data.get<const char*>("font")), al_map_rgba(col.get<unsigned char>("r"), col.get<unsigned char>("g"), col.get<unsigned char>("b"), col.get<unsigned char>("a")));
		});
	}

	void TextureAtlas::addRectToAtlas(const std::string& id, const Rect<int>& rect)
	{
		m_resourceMap.emplace(entt::HashedString{ id.c_str() }, rect);
	}

	void TextureAtlas::al_draw_packed_bitmap(const std::string& texture, float dx, float dy, int flags)
	{
		// Get rectangle coords of texture on atlas.
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];

		// Draw that texture on the atlas.
		al_draw_bitmap_region(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float dx, float dy, int flags)
	{
		// Get rectangle coords of texture on atlas.
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];

		// Draw that texture on the atlas.
		al_draw_tinted_bitmap_region(m_atlas, tint, pr.m_x, pr.m_y, pr.m_width, pr.m_height, dx, dy, flags);
	}

	void TextureAtlas::al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& texture, ALLEGRO_COLOR tint, float cx, float cy, float dx, float dy, float xscale, float yscale, float angle, int flags)
	{
		// Get rectangle coords of texture on atlas.
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];

		// Draw that texture on the atlas.
		al_draw_tinted_scaled_rotated_bitmap_region(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height, tint, cx, cy, dx, dy, xscale, yscale, Box2DHelper::degToRad<float>(angle), flags);
	}

	void TextureAtlas::clean()
	{
		al_destroy_bitmap(m_atlas);
	}

	ALLEGRO_BITMAP* TextureAtlas::al_create_packed_sub_bitmap(const std::string& texture)
	{
		// Get rectangle coords of texture on atlas.
		auto pr = m_resourceMap[entt::HashedString{ texture.c_str() }];

		// Return sub-bitmap of that texture from the atlas.
		return al_create_sub_bitmap(m_atlas, pr.m_x, pr.m_y, pr.m_width, pr.m_height);
	}

	void TextureAtlas::drawInternalTexture(float x, float y)
	{
		// Draw the entire atlas to the screen. Useful for debugging.
		al_draw_bitmap(m_atlas, x, y, 0);
	}
}