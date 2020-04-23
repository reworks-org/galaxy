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
		m_textureFiles.clear();
		m_idMap.clear();
	}

	void TextureAtlas::add(const std::string& name) noexcept
	{
		m_textureFiles.push_back(name);
	}

	void TextureAtlas::create(qs::Window& window, qs::Renderer& renderer, qs::Shader& shader) noexcept
	{
		if (!m_textureFiles.empty())
		{
			qs::VertexQuadStorage vertexs;
			vertexs.reserve(m_textureFiles.size() * 4);

			m_texture.bind();

			unsigned int counter = 0;
			for (const auto& file : m_textureFiles)
			{
				auto filePath = std::filesystem::path(file);

				// Load texture.
				qs::Sprite loadedTex;
				loadedTex.load(filePath.string());
				loadedTex.create(qs::BufferType::DYNAMIC);

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(loadedTex.getWidth(), loadedTex.getHeight());
				if (opt == std::nullopt)
				{
					qs::Error::handle().callback("TextureAtlas.cpp", 68, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					loadedTex.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));
					loadedTex.applyTransforms();

					renderer.drawSpriteToTexture(loadedTex, m_texture, shader);

					auto quad = qs::Vertex::make_quad(
						{ 0.0f, 0.0f, static_cast<float>(rect.m_width), static_cast<float>(rect.m_height) },
						{ 0.0f, 0.0f, 0.0f, 1.0f },
						static_cast<float>(rect.m_x), static_cast<float>(rect.m_y)
					);

					m_idMap.emplace(filePath.stem().string(), counter * 4);
					vertexs.push_back(quad);
					counter++;
				}
			}
			
			m_texture.unbind(window);
			m_sprite.load(m_texture.getGLTexture(), m_texture.getWidth(), m_texture.getHeight());
			m_sprite.create(vertexs);
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 96, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::save(const std::string& file)
	{
		m_texture.save(file);
	}

	const int TextureAtlas::getID(const std::string& name) noexcept
	{
		return m_idMap[name];
	}

	const int TextureAtlas::getSize() const noexcept
	{
		return m_size;
	}

	qs::Sprite& TextureAtlas::getSprite() noexcept
	{
		return m_sprite;
	}

	qs::RenderTexture& TextureAtlas::getTexture() noexcept
	{
		return m_texture;
	}
}