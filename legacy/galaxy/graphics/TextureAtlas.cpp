///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/sprite/Sprite.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace graphics
	{
		TextureAtlas::TextureAtlas()
		    : m_size {4096}, m_texture {4096, 4096}
		{
			m_packer.init(m_size, m_size);
			m_texture.create(m_size, m_size);
		}

		TextureAtlas::TextureAtlas(const unsigned int size)
		    : m_size {size}, m_texture {size, size}
		{
			if (size == 0)
			{
				throw std::runtime_error("TextureAtlas size cannot be 0.");
			}

			if (!((size & (size - 1)) == 0))
			{
				throw std::runtime_error("TextureAtlas size is not power of 2.");
			}

			m_packer.init(size, size);
			m_texture.create(m_size, m_size);
		}

		TextureAtlas::TextureAtlas(TextureAtlas&& ta)
		{
			this->m_size     = ta.m_size;
			this->m_texture  = std::move(ta.m_texture);
			this->m_packer   = std::move(ta.m_packer);
			this->m_textures = std::move(m_textures);
		}

		TextureAtlas& TextureAtlas::operator=(TextureAtlas&& ta)
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

		TextureAtlas::~TextureAtlas()
		{
			m_textures.clear();
		}

		void TextureAtlas::add(std::string_view file)
		{
			const auto path = std::filesystem::path {file};
			const auto name = path.stem().string();
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

		void TextureAtlas::create(Renderer& renderer, Shader& shader)
		{
			if (!m_textures.empty())
			{
				m_texture.bind();

				for (auto& [name, info] : m_textures)
				{
					// Load texture.
					Sprite to_draw;
					to_draw.load(info.m_path.string());
					to_draw.create<BufferDynamic>();

					// Pack into rect then add to hashmap.
					auto opt = m_packer.pack(to_draw.get_width(), to_draw.get_height());
					if (opt == std::nullopt)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to pack texture: {0}.", name);
					}
					else
					{
						auto rect = opt.value();
						to_draw.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

						renderer.draw_sprite_to_texture(&to_draw, m_texture, shader);

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

		void TextureAtlas::update(Renderer& renderer, Shader& shader)
		{
			if (!m_textures.empty())
			{
				m_texture.change_size(m_size, m_size);
				create(renderer, shader);
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

		std::optional<graphics::Rect<float>> TextureAtlas::get_region(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (!m_textures.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Tried to access texture rect that does not exist.");
				return std::nullopt;
			}
			else
			{
				return std::make_optional(m_textures[str].m_region);
			}
		}

		RenderTexture* TextureAtlas::get_atlas()
		{
			return &m_texture;
		}

		TextureAtlas::AtlasTextureData& TextureAtlas::get_tex_data()
		{
			return m_textures;
		}

		const int TextureAtlas::get_size() const
		{
			return m_size;
		}

		const unsigned int TextureAtlas::gl_texture() const
		{
			return m_texture.gl_texture();
		}
	} // namespace graphics
} // namespace galaxy