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

#include "TextureAtlas.hpp"

namespace qs
{
	TextureAtlas::TextureAtlas() noexcept
	    : m_size {1024}, m_texture {1024, 1024}
	{
		// This is the default size.
		m_packer.init(1024, 1024);
	}

	TextureAtlas::TextureAtlas(const unsigned int size)
	    : m_size {size}, m_texture {size, size}
	{
		if (size == 0 || !((size & (size - 1)) == 0))
		{
			throw std::runtime_error("TextureAtlas size is not power of 2.");
		}

		m_packer.init(size, size);
	}

	TextureAtlas::~TextureAtlas() noexcept
	{
		m_textures.clear();
		m_regions.clear();
	}

	void TextureAtlas::add(std::string_view name) noexcept
	{
		m_textures.emplace_back(name);
	}

	void TextureAtlas::create(qs::Renderer& renderer, qs::Shader& shader)
	{
		if (!m_textures.empty())
		{
			m_texture.bind();

			for (const auto& file : m_textures)
			{
				auto path = std::filesystem::path {file};

				// Load texture.
				qs::Sprite to_draw;
				to_draw.load(path.string());
				to_draw.create<qs::BufferDynamic>();

				// Pack into rect then add to hashmap.
				auto opt = m_packer.pack(to_draw.get_width(), to_draw.get_height());
				if (opt == std::nullopt)
				{
					PL_LOG(PL_ERROR, "Failed to pack texture: {0}.", file);
				}
				else
				{
					auto rect = opt.value();
					to_draw.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

					renderer.draw_sprite_to_texture(&to_draw, m_texture, shader);

					m_regions[path.stem().string()] = {static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), static_cast<float>(rect.m_width), static_cast<float>(rect.m_height)};
				}
			}

			m_texture.unbind();
		}
		else
		{
			PL_LOG(PL_ERROR, "Tried to create atlas with no texture files!");
		}
	}

	void TextureAtlas::save(std::string_view file)
	{
		m_texture.save(file);
	}

	void TextureAtlas::add_custom_quad(std::string_view name, const pr::Rect<float>& rect) noexcept
	{
		const auto str = static_cast<std::string>(name);
		if (!m_regions.contains(str))
		{
			m_regions[str] = rect;
		}
		else
		{
			PL_LOG(PL_WARNING, "Could not add duplicate texture region: {0}.", str);
		}
	}

	std::optional<pr::Rect<float>> TextureAtlas::get_region(std::string_view name) noexcept
	{
		const auto str = static_cast<std::string>(name);
		if (m_regions.contains(str))
		{
			return std::make_optional(m_regions[str]);
		}
		else
		{
			PL_LOG(PL_WARNING, "Tried to access texture rect that does not exist.");
			return std::nullopt;
		}
	}

	qs::RenderTexture* TextureAtlas::get_atlas() noexcept
	{
		return &m_texture;
	}

	const int TextureAtlas::get_size() const noexcept
	{
		return m_size;
	}
} // namespace qs