///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/scripting/JSONUtils.hpp"
#include "galaxy/res/ShaderBook.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace res
	{
		TextureAtlas::TextureAtlas()
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);

			m_packer.init(m_size, m_size);
			m_texture.create(m_size, m_size);
		}

		TextureAtlas::TextureAtlas(std::string_view file)
		{
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);

			m_packer.init(m_size, m_size);
			m_texture.create(m_size, m_size);

			add_from_json(file);
		}

		TextureAtlas::TextureAtlas(TextureAtlas&& ta) noexcept
		{
			this->m_size     = ta.m_size;
			this->m_texture  = std::move(ta.m_texture);
			this->m_packer   = std::move(ta.m_packer);
			this->m_textures = std::move(m_textures);
		}

		TextureAtlas& TextureAtlas::operator=(TextureAtlas&& ta) noexcept
		{
			if (this != &ta)
			{
				this->m_size     = ta.m_size;
				this->m_texture  = std::move(ta.m_texture);
				this->m_packer   = std::move(ta.m_packer);
				this->m_textures = std::move(m_textures);
			}

			return *this;
		}

		TextureAtlas::~TextureAtlas() noexcept
		{
			m_textures.clear();
		}

		void TextureAtlas::add(std::string_view file)
		{
			std::string path = SL_HANDLE.vfs()->absolute(file);
			const auto name  = std::filesystem::path(path).stem().string();
			if (!m_textures.contains(name))
			{
				m_textures[name] = {.m_path = path};
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to add pre-existing texture.");
			}
		}

		void TextureAtlas::add_from_json(std::string_view json)
		{
			auto j        = json::parse_from_disk(static_cast<std::string>(json));
			auto textures = j.at("textures");

			std::for_each(textures.begin(), textures.end(), [&](const nlohmann::json& texture) {
				add(texture.get<std::string>());
			});
		}

		void TextureAtlas::create(std::string_view shader)
		{
			if (!m_textures.empty())
			{
				m_texture.bind();

				for (auto& [name, info] : m_textures)
				{
					// Load texture.
					components::Transform to_draw_transform;
					components::Sprite to_draw;
					to_draw.load(info.m_path);
					to_draw.create();

					// Pack into rect then add to hashmap.
					const auto opt = m_packer.pack(to_draw.get_width(), to_draw.get_height());
					if (opt == std::nullopt)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to pack texture: {0}.", name);
					}
					else
					{
						auto rect = opt.value();
						to_draw_transform.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

						auto* shader_ptr = SL_HANDLE.shaderbook()->get(shader);
						graphics::Renderer::draw_sprite_to_texture(&to_draw, &to_draw_transform, shader_ptr, &m_texture);

						info.m_region = {static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), static_cast<float>(rect.m_width), static_cast<float>(rect.m_height)};
					}
				}

				m_texture.unbind();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create atlas with no texture files!");
			}
		}

		void TextureAtlas::update(std::string_view shader)
		{
			if (!m_textures.empty())
			{
				m_texture.change_size(m_size, m_size);
				create(shader);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create atlas with no texture files!");
			}
		}

		void TextureAtlas::save(std::string_view file)
		{
			m_texture.save(file);
		}

		const graphics::fRect& TextureAtlas::get_region(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (!m_textures.contains(str))
			{
				GALAXY_LOG(GALAXY_FATAL, "Tried to access texture rect that does not exist.");
			}
			else
			{
				return m_textures[str].m_region;
			}
		}

		TextureAtlas::AtlasTextureData& TextureAtlas::get_tex_data() noexcept
		{
			return m_textures;
		}

		graphics::RenderTexture* TextureAtlas::get_atlas() noexcept
		{
			return &m_texture;
		}

		const int TextureAtlas::get_size() const noexcept
		{
			return m_size;
		}

		const unsigned int TextureAtlas::gl_texture() const noexcept
		{
			return m_texture.gl_texture();
		}
	} // namespace res
} // namespace galaxy