///
/// TextureAtlas.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/core/Shader.hpp"

#include "TextureAtlas.hpp"

namespace qs
{
	TextureAtlas::TextureAtlas() noexcept
		:m_size(1024), m_atlas(1024, 1024)
	{
		// Defaults to 512.
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

	void TextureAtlas::create(qs::Shader& shader, qs::Window& window) noexcept
	{
		if (!m_textureFiles.empty())
		{
			shader.use();
			m_atlas.activate(shader);

			float counter = 0;
			for (const auto& file : m_textureFiles)
			{
				// Load texture.
				qs::Sprite2D tex;

				auto filePath = std::filesystem::path(file);
				tex.load(filePath.string());
				tex.create();

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(tex.getWidth(), tex.getHeight());
				if (opt == std::nullopt)
				{
					qs::Error::handle().callback("TextureAtlas.cpp", 60, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					m_batchSprite.push({
						0.0f, 0.0f, cntp(rect.m_x), cntp(rect.m_y), counter,
						(float)rect.m_width, 0.0f, cntp(rect.m_x + rect.m_width), cntp(rect.m_y), counter,
						(float)rect.m_width, (float)rect.m_height, cntp(rect.m_x + rect.m_width), cntp(rect.m_y + rect.m_height), counter,
						0.0f, (float)rect.m_height, cntp(rect.m_x), cntp(rect.m_y + rect.m_height), counter
					});
					 
					// Draw to master texture
					tex.setPos(rect.m_x, rect.m_y);
					shader.setUniform("u_transform", tex.getTransformation());

					tex.activate();
					glDrawElements(GL_TRIANGLES, tex.getIBO().count(), GL_UNSIGNED_INT, nullptr);

					m_batchSprite.addID(filePath.filename().string(), counter);
					counter += 1;
				}
			}

			m_atlas.deactivate(window);
			m_batchSprite.load(m_atlas.getSprite().getGLTexture(), m_atlas.getWidth(), m_atlas.getHeight());
			m_batchSprite.create(counter);
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 89, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::dump(const std::string& file)
	{
		m_atlas.getSprite().save(file);
	}

	qs::Sprite2D& TextureAtlas::getAtlas() noexcept
	{
		return m_atlas.getSprite();
	}

	qs::BatchSprite2D* TextureAtlas::getBatch() noexcept
	{
		return &m_batchSprite;
	}

	float TextureAtlas::cntp(const int p) noexcept
	{
		return (static_cast<float>(p) / static_cast<float>(m_size));
	}
}