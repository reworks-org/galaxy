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

#define DEFAULT_ATLAS_SIZE 4096

namespace galaxy
{
	namespace res
	{
		TextureAtlas::TextureAtlas()
		{
			m_rect_packs.emplace_back();
			m_rect_packs[0].init(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);
			m_rendertextures.emplace_back(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);
		}

		TextureAtlas::TextureAtlas(std::string_view file)
		{
			m_rect_packs.emplace_back();
			m_rect_packs[0].init(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);
			m_rendertextures.emplace_back(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);

			build_from_json(file);
		}

		TextureAtlas::TextureAtlas(TextureAtlas&& ta) noexcept
		{
			this->m_rect_packs     = std::move(ta.m_rect_packs);
			this->m_rendertextures = std::move(ta.m_rendertextures);
			this->m_textures       = std::move(ta.m_textures);
		}

		TextureAtlas& TextureAtlas::operator=(TextureAtlas&& ta) noexcept
		{
			if (this != &ta)
			{
				this->m_rect_packs     = std::move(ta.m_rect_packs);
				this->m_rendertextures = std::move(ta.m_rendertextures);
				this->m_textures       = std::move(ta.m_textures);
			}

			return *this;
		}

		TextureAtlas::~TextureAtlas() noexcept
		{
			m_textures.clear();
			m_rect_packs.clear();
			m_rendertextures.clear();
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

		void TextureAtlas::build_from_json(std::string_view json)
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
				for (auto& [name, info] : m_textures)
				{
					// Load texture.
					components::Transform to_draw_transform;
					components::Sprite to_draw;
					to_draw.load(info.m_path);
					to_draw.create();

					// Pack into rect then add to hashmap.
					std::optional<graphics::iRect> result = std::nullopt;
					std::size_t counter                   = 0;
					while (result == std::nullopt)
					{
						result = m_rect_packs[counter].pack(to_draw.get_width(), to_draw.get_height());
						if (result != std::nullopt)
						{
							m_rendertextures[counter].bind();
							const auto& rect = result.value();
							to_draw_transform.move(static_cast<float>(rect.m_x), static_cast<float>(rect.m_y));

							auto* shader_ptr = SL_HANDLE.shaderbook()->get(shader);
							graphics::Renderer::draw_sprite_to_texture(&to_draw, &to_draw_transform, shader_ptr, &m_rendertextures[counter]);

							info.m_region   = {static_cast<float>(rect.m_x), static_cast<float>(rect.m_y), static_cast<float>(rect.m_width), static_cast<float>(rect.m_height)};
							info.m_sheet_id = counter;
							m_rendertextures[counter].unbind();
						}
						else
						{
							counter++;
							if (counter >= m_rect_packs.size())
							{
								m_rect_packs.emplace_back().init(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);
								m_rendertextures.emplace_back(DEFAULT_ATLAS_SIZE, DEFAULT_ATLAS_SIZE);
							}
						}
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to create atlas with no texture files!");
			}
		}

		void TextureAtlas::save(std::string_view file, int index)
		{
			if (index == -1)
			{
				std::size_t counter = 0;
				for (auto& rt : m_rendertextures)
				{
					const auto filename = static_cast<std::string>(file) + std::to_string(counter);
					rt.save(filename);
					counter++;
				}
			}
			else
			{
				if (index < m_rendertextures.size())
				{
					m_rendertextures[index].save(file);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Attempted to save non-existent texture atlas: {0}. Maxmimum saveable index is: {1}.", index, m_rendertextures.size() - 1);
				}
			}
		}

		graphics::TextureInfo& TextureAtlas::get_tex_info(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (!m_textures.contains(str))
			{
				GALAXY_LOG(GALAXY_FATAL, "Tried to access texture rect that does not exist.");
			}
			else
			{
				return m_textures[str];
			}
		}

		graphics::RenderTexture* const TextureAtlas::get_atlas(const std::size_t index)
		{
			if (index < m_rendertextures.size())
			{
				return &m_rendertextures[index];
			}
			else
			{
				GALAXY_LOG(GALAXY_FATAL, "Attempted to access non-existent texture atlas: {0}. Maxmimum index is: {1}.", index, m_rendertextures.size() - 1);
				return nullptr;
			}
		}

		TextureAtlas::AtlasTextureData& TextureAtlas::get_tex_data() noexcept
		{
			return m_textures;
		}

		const constexpr int TextureAtlas::get_size() const noexcept
		{
			return DEFAULT_ATLAS_SIZE;
		}
	} // namespace res
} // namespace galaxy