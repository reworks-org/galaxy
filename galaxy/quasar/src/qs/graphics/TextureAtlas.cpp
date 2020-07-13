///
/// TextureAtlas.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <pulsar/Log.hpp>

#include "qs/core/Shader.hpp"
#include "qs/core/Renderer.hpp"
#include "qs/sprite/Sprite.hpp"
#include "qs/utils/Utility.hpp"

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

	void TextureAtlas::create(qs::Renderer& renderer, qs::Shader& shader) noexcept
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
					PL_LOG(PL_WARNING, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					loadedTex.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

					renderer.drawSpriteToTexture(&loadedTex, m_texture, shader);

					m_textureRects.emplace(filePath.stem().string(), pr::Rect<float>(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), static_cast<float>(rect.m_width), static_cast<float>(rect.m_height)));
				}
			}
			
			m_texture.unbind();
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::save(const std::string& file) noexcept
	{
		m_texture.save(file);
	}

	const int TextureAtlas::getSize() const noexcept
	{
		return m_size;
	}

	void TextureAtlas::defineCustomQuad(std::string_view name, const pr::Rect<float>& rect) noexcept
	{
		const std::string cast = static_cast<std::string>(name);
		if (m_textureRects.find(cast) == m_textureRects.end())
		{
			m_textureRects.emplace(cast, rect);
		}
	}

	const pr::Rect<float>& TextureAtlas::getTexQuad(std::string_view name) noexcept
	{
		const std::string cast = static_cast<std::string>(name);
		if (m_textureRects.find(cast) != m_textureRects.end())
		{
			return m_textureRects[cast];
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access texture rect that does not exist. Returning blank rect...");
			return std::move(pr::Rect<float>{ 0.0f, 0.0f, 0.0f, 0.0f });
		}
	}

	qs::RenderTexture* TextureAtlas::getTexture() noexcept
	{
		return &m_texture;
	}
}