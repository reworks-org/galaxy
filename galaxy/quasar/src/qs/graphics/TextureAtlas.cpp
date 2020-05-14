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
#include "qs/graphics/Sprite.hpp"
#include "qs/renderer/Renderer.hpp"

#include "TextureAtlas.hpp"

namespace qs
{
	TextureAtlas::TextureAtlas() noexcept
		:m_size(1024), m_texture(1024, 1024)
	{
		// This is the default size.
		m_packer.init(1024, 1024);
	}

	TextureAtlas::TextureAtlas(const int size) noexcept
		:m_size(size), m_texture(size, size)
	{
		m_packer.init(size, size);
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		m_textureRects.clear();
	}

	void TextureAtlas::add(const std::string& name) noexcept
	{
		m_textureFiles.push_back(name);
	}

	void TextureAtlas::create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept
	{
		if (!m_textureFiles.empty())
		{
			m_texture.bind();

			for (const auto& file : m_textureFiles)
			{
				auto filePath = std::filesystem::path(file);

				// Load texture.
				qs::Sprite loadedTex;
				loadedTex.load(filePath.string());
				loadedTex.create<qs::BufferTypeDynamic>();

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(loadedTex.getWidth(), loadedTex.getHeight());
				if (opt == std::nullopt)
				{
					qs::Error::handle().callback("TextureAtlas.cpp", 63, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					loadedTex.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

					renderer.drawSpriteToTexture(loadedTex, m_texture, shader);

					m_textureRects.emplace(filePath.stem().string(), protostar::Rect<float>(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), static_cast<float>(rect.m_width), static_cast<float>(rect.m_height)));
				}
			}
			
			m_texture.unbind(window);
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 80, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::save(const std::string& file) noexcept
	{
		m_texture.save(file);
	}

	const protostar::Rect<float>& TextureAtlas::getTexQuad(const std::string& name) noexcept
	{
		if (m_textureRects.find(name) != m_textureRects.end())
		{
			return m_textureRects[name];
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 97, "Tried to access texture rect that does not exist. Returning blank rect...");
			protostar::Rect<float> temp = { 0.0f, 0.0f, 0.0f, 0.0f };
			return std::move(temp);
		}
	}

	const int TextureAtlas::getSize() const noexcept
	{
		return m_size;
	}

	qs::RenderTexture& TextureAtlas::getTexture() noexcept
	{
		return m_texture;
	}
}