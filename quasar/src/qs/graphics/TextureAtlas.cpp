///
/// TextureAtlas.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>


#include "qs/core/Shader.hpp"
#include "qs/utils/Utility.hpp"
#include "qs/graphics/Sprite2D.hpp"
#include "qs/renderer/Renderer.hpp"

#include "TextureAtlas.hpp"

namespace qs
{
	TextureAtlas::TextureAtlas() noexcept
		:m_size(1024), m_atlas(1024, 1024)
	{
		// This is the default size.
		m_packer.init(1024, 1024);
	}

	TextureAtlas::TextureAtlas(const int size) noexcept
		:m_size(size), m_atlas(size, size)
	{
		m_packer.init(size, size);
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		m_textureFiles.clear();
	}

	void TextureAtlas::add(const std::string& name) noexcept
	{
		m_textureFiles.push_back(name);
	}

	void TextureAtlas::create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept
	{
		if (!m_textureFiles.empty())
		{
			float counter = 0.0f;
			for (const auto& file : m_textureFiles)
			{
				auto filePath = std::filesystem::path(file);

				// Load texture.
				qs::Sprite2D tex;
				tex.load(filePath.string());
				tex.create();

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(tex.getWidth(), tex.getHeight());
				if (opt == std::nullopt)
				{
					qs::Error::handle().callback("TextureAtlas.cpp", 62, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					/*
						m_batchSprite.push({
						0.0f, 1.0f, Utils::toNorm(rect.m_x, m_size), Utils::toNorm(rect.m_y, m_size), counter,
						static_cast<float>(rect.m_width), 1.0f, Utils::toNorm(rect.m_x + rect.m_width, m_size), Utils::toNorm(rect.m_y, m_size), counter,
						static_cast<float>(rect.m_width), static_cast<float>(rect.m_height), Utils::toNorm(rect.m_x + rect.m_width, m_size), Utils::toNorm(rect.m_y + rect.m_height, m_size), counter,
						0.0f, static_cast<float>(rect.m_height), Utils::toNorm(rect.m_x, m_size), Utils::toNorm(rect.m_y + rect.m_height, m_size), counter
					});
					*/
					
					m_atlas.bind();
					tex.setPos(rect.m_x, rect.m_y);
					renderer.drawSpriteToTexture(tex, m_atlas, shader);
					counter += 1.0f;
				}
			}
			
			m_atlas.unbind(window);
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 91, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::save(const std::filesystem::path& file)
	{
		m_atlas.save(file.string());
	}
}