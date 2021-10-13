///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform2D.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/meta/StaticIDGen.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace graphics
	{
		TextureAtlas::TextureAtlas() noexcept
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);
			m_size = std::min(m_size, 4096);

			m_id = meta::StaticIDGen<TextureAtlas>::get();
			m_packer.init(m_size, m_size);

			m_render_texture.create(m_size, m_size);
			m_render_texture.clear();
		}

		TextureAtlas::TextureAtlas(std::string_view file)
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);
			m_size = std::min(m_size, 4096);

			m_id = meta::StaticIDGen<TextureAtlas>::get();
			m_packer.init(m_size, m_size);

			m_render_texture.create(m_size, m_size);
			m_render_texture.clear();

			add_json(file);
		}

		TextureAtlas::TextureAtlas(TextureAtlas&& ta) noexcept
		{
			this->m_id             = ta.m_id;
			this->m_packer         = std::move(ta.m_packer);
			this->m_render_texture = std::move(ta.m_render_texture);
			this->m_size           = ta.m_size;
			this->m_textures       = std::move(ta.m_textures);

			ta.m_id = 0;
		}

		TextureAtlas& TextureAtlas::operator=(TextureAtlas&& ta) noexcept
		{
			if (this != &ta)
			{
				this->m_id             = ta.m_id;
				this->m_packer         = std::move(ta.m_packer);
				this->m_render_texture = std::move(ta.m_render_texture);
				this->m_size           = ta.m_size;
				this->m_textures       = std::move(ta.m_textures);

				ta.m_id = 0;
			}

			return *this;
		}

		TextureAtlas::~TextureAtlas() noexcept
		{
			m_textures.clear();
		}

		const bool TextureAtlas::add(std::string_view file)
		{
			bool result = true;

			const auto path_opt = SL_HANDLE.vfs()->absolute(file);
			if (path_opt != std::nullopt)
			{
				const auto& path = std::filesystem::path(path_opt.value());
				const auto& name = path.stem();

				if (!m_textures.contains(name.string()))
				{
					// Pack into rect then add to hashmap.
					components::Sprite to_draw_spr;
					to_draw_spr.load(path.string());
					to_draw_spr.create("bg");

					const auto opt = m_packer.pack(to_draw_spr.get_width(), to_draw_spr.get_height());
					if (opt != std::nullopt)
					{
						m_render_texture.bind(false);

						// Load texture.
						components::Transform2D to_draw_tf;
						to_draw_tf.move(static_cast<float>(opt.value().m_x), static_cast<float>(opt.value().m_y));

						RENDERER_2D().bind_rtt();
						RENDERER_2D().draw_sprite_to_target(&to_draw_spr, &to_draw_tf, &m_render_texture);

						m_textures[name.string()] = {.m_region = {static_cast<float>(opt.value().m_x),
														 static_cast<float>(opt.value().m_y),
														 static_cast<float>(opt.value().m_width),
														 static_cast<float>(opt.value().m_height)},
							.m_path                            = path.string(),
							.m_index                           = m_id};

						to_draw_spr.unbind();
						m_render_texture.unbind();
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to pack texture: {0}.", name.string());
						result = false;
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to add pre-existing texture.");
					result = false;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find texture: {0}, to add to atlas.", file);
				result = false;
			}

			return result;
		}

		void TextureAtlas::add_multi(std::span<std::string> files)
		{
			for (const auto& file : files)
			{
				// Error handling is done inside add().
				add(file);
			}
		}

		void TextureAtlas::add_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt != std::nullopt)
			{
				const auto& json     = json_opt.value();
				const auto& textures = json.at("textures");

				std::for_each(textures.begin(), textures.end(), [&](const nlohmann::json& texture) {
					add(texture.get<std::string>());
				});
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for ", file);
			}
		}

		void TextureAtlas::add_custom_region(std::string_view key, const math::Rect<float>& region)
		{
			if (contains(key))
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to add existing texture region as custom region: {0}.", key);
			}
			else
			{
				graphics::TextureInfo info {.m_region = region, .m_path = "", .m_index = m_id};

				m_textures.emplace(static_cast<std::string>(key), info);
			}
		}

		void TextureAtlas::save(std::string_view file_name)
		{
			m_render_texture.save(file_name);
		}

		const bool TextureAtlas::contains(std::string_view key) noexcept
		{
			return m_textures.contains(static_cast<std::string>(key));
		}

		std::optional<TextureInfo> TextureAtlas::get_texture_info(std::string_view key) noexcept
		{
			if (contains(key))
			{
				return m_textures[static_cast<std::string>(key)];
			}
			else
			{
				return std::nullopt;
			}
		}

		const int TextureAtlas::get_size() const noexcept
		{
			return m_size;
		}

		const std::size_t TextureAtlas::get_id() const noexcept
		{
			return m_id;
		}

		const unsigned int TextureAtlas::gl_texture() const noexcept
		{
			return m_render_texture.get_texture();
		}
	} // namespace graphics
} // namespace galaxy