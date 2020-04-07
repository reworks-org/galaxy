///
/// TextureAtlas.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/graphics/RenderTexture.hpp"

#include "TextureAtlas.hpp"

namespace qs
{
	TextureAtlas::TextureAtlas() noexcept
	{
		// Defaults to 512.
		m_packer.init(512, 512);
	}

	TextureAtlas::TextureAtlas(const int size) noexcept
	{
		m_packer.init(size, size);
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		m_rects.clear();
	}

	void TextureAtlas::add(const std::string& name) noexcept
	{
		m_textureFiles.push_back(name);
	}

	void TextureAtlas::create(qs::Shader& shader) noexcept
	{
		if (!m_textureFiles.empty())
		{
			qs::RenderTexture rt(m_packer.getWidth(), m_packer.getHeight());
			rt.activate(shader);

			for (const auto& file : m_textureFiles)
			{
				// Load texture.
				qs::Sprite2D tex;
				tex.load(file);

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(tex.getWidth(), tex.getHeight());
				if (opt == std::nullopt)
				{
					qs::Error::handle().callback("TextureAtlas.cpp", 53, "Failed to pack texture: " + file);
				}
				else
				{
					auto rect = opt.value();
					m_rects.emplace(file, rect);
					tex.addVertexes<16>({
						static_cast<float>(rect.m_x + rect.m_width), static_cast<float>(rect.m_y), 1.0f, 0.0f, // top right pos
						static_cast<float>(rect.m_x + rect.m_width), static_cast<float>(rect.m_y + rect.m_height), 1.0f, 1.0f, // bottom right
						static_cast<float>(rect.m_x), static_cast<float>(rect.m_y + rect.m_height),	0.0f, 1.0f, // bottom left
						static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), 0.0f, 0.0f // top left
					});

					tex.addIndexes<0>({});
					tex.addLayout(4, { 2, 2 });
					tex.create();
					
					// Draw to master texture.
					tex.activate();
					shader.setUniform("u_transform", tex.getTransformation());

					glDrawElements(GL_TRIANGLES, tex.getIBO().count(), GL_UNSIGNED_INT, nullptr);
				}
			}

			rt.deactivate();

			m_atlas.load(rt.getTexture(), rt.getWidth(), rt.getHeight());
		}
		else
		{
			qs::Error::handle().callback("TextureAtlas.cpp", 84, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::dump(const std::string& file)
	{
		m_atlas.save(file);
	}

	protostar::Rect<int> TextureAtlas::getRect(const std::string& texture) noexcept
	{
		return m_rects[texture];
	}

	qs::Sprite2D TextureAtlas::getSprite(const std::string& texture) noexcept
	{
		return qs::Sprite2D();
	}

	qs::Sprite2D& TextureAtlas::getAtlas() noexcept
	{
		return m_atlas;
	}
}