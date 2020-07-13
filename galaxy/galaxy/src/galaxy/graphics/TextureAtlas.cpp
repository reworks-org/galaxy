///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <pl/Log.hpp>
#include <nlohmann/json.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/PhysfsInputStream.hpp"


#include "TextureAtlas.hpp"

namespace galaxy
{
	/*TextureAtlas::TextureAtlas(const std::string& folder)
		:m_nullTexture("")
	{
		// Create atlas size from power of 2, since the algorithms work best with power of 2's.
		int atlasSize = std::pow(2, powerOfTwoDimension);
		m_atlas = al_create_bitmap(atlasSize, atlasSize);

		// Set the algorithm to use when packing the bin, in this case bestshortsidefit.
		m_bin.Init(atlasSize, atlasSize, false);

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
				std::string loc = textureFolderPath + std::string(*i);
				ALLEGRO_BITMAP* bitmap = al_load_bitmap(loc.c_str());

				// ...then fit it into the atlas...
				Rect<int> packedRect = m_bin.Insert(al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), rbp::MaxRectsBinPack::RectBestAreaFit);
				if (!(packedRect.m_height > 0))
				{
					LOG_S(FATAL) << "Failed to pack a texture! Texture: " << *i;
				}

				// ...then draw it to the atlas bitmap.
				al_draw_bitmap(bitmap, packedRect.m_x, packedRect.m_y, 0);
				
				m_resourceMap.emplace(entt::HashedString(std::filesystem::path(*i).stem().string().c_str()), packedRect);

				al_destroy_bitmap(bitmap);
			}
		}
		
		// Clean up, ensuring the display is set correctly.
		al_flip_display();
		al_set_target_bitmap(al_get_backbuffer(Locator::window->getDisplay()));
		
		// Free up the list.
		PHYSFS_freeList(efl);
	}*/

	TextureAtlas::TextureAtlas() noexcept
	{
		// Start with 512.
		m_packer.init(512, 512);
	}

	void TextureAtlas::add(const std::string& name) noexcept
	{
		auto fs = SL_HANDLE.fs();
	}

	void TextureAtlas::addFromJSON(const std::string& json)
	{
		auto fs = SL_HANDLE.fs();

		if (fs->has(json))
		{
			std::string code = fs->read(json);
			if (!code.empty())
			{
				sf::RenderTexture rt;
				if (!rt.create(m_packer.getWidth(), m_packer.getHeight()))
				{
					PL_LOG(pl::Log::Level::ERROR_, "Failed to create RenderTexture.");
				}
				else
				{
					rt.clear(sf::Color::Transparent);

					nlohmann::json parsed = nlohmann::json::parse(code);
					std::for_each(parsed.begin(), parsed.end(), [&](const nlohmann::json& array)
					{
						std::string name = array["name"];
						std::string file = array["file"];
						 
						// Make sure is destroyed upon leaving scope.
						{
							// Load texture.
							sf::Texture tex;
							tex.loadFromStream(galaxy::PhysfsInputStream(file));

							// Pack into rect then add to hashmap.
							auto rect = m_packer.pack(tex.getSize().x, tex.getSize().y);
							if (rect == std::nullopt)
							{
								PL_LOG(pl::Log::Level::ERROR_, "Failed to pack texture: " + name);
							}
							else
							{
								m_rects.emplace(name, rect.value());
								
								// Draw to master texture.
								rt.draw(sf::Sprite(tex));
							}
						}
					});

					rt.display();

					m_atlas = rt.getTexture();
				}
			}
			else
			{
				PL_LOG(pl::Log::Level::ERROR_, "File read was empty: " + json);
			}
		}
		else
		{
			PL_LOG(pl::Log::Level::ERROR_, "Virtual FS does not contain: " + json);
		}
	}

	void TextureAtlas::dump(const std::string& file)
	{
		m_atlas.copyToImage().saveToFile(file);
	}

	pr::Rect<int> TextureAtlas::getRect(const std::string& texture) noexcept
	{
		return m_rects[texture];
	}

	sf::Sprite TextureAtlas::getSprite(const std::string& texture) noexcept
	{
		// Have to convert to SFML rect.
		sf::IntRect ir;
		ir.left = m_rects[texture].m_x;
		ir.top = m_rects[texture].m_y;
		ir.width = m_rects[texture].m_width;
		ir.height = m_rects[texture].m_height;

		return sf::Sprite(m_atlas, ir);
	}
	
	sf::Texture& TextureAtlas::getAtlas() noexcept
	{
		return m_atlas;
	}
}