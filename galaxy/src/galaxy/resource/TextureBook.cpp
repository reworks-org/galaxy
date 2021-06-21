///
/// TextureBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include <nlohmann/json.hpp>

#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/scripting/JSONUtils.hpp"
#include "galaxy/resource/ShaderBook.hpp"

#include "TextureBook.hpp"

namespace galaxy
{
	namespace res
	{
		TextureBook::TextureBook(std::string_view file)
		{
			add_json(file);
		}

		TextureBook::TextureBook(TextureBook&& tb) noexcept
		{
			this->m_atlas   = std::move(tb.m_atlas);
			this->m_batches = std::move(tb.m_batches);
		}

		TextureBook& TextureBook::operator=(TextureBook&& tb) noexcept
		{
			if (this != &tb)
			{
				this->m_atlas   = std::move(tb.m_atlas);
				this->m_batches = std::move(tb.m_batches);
			}

			return *this;
		}

		TextureBook::~TextureBook() noexcept
		{
			clear();
		}

		const bool TextureBook::add(std::string_view file)
		{
			bool result = false;

			for (const auto& atlas : m_atlas)
			{
				result = add(file);
				if (result)
				{
					break;
				}
			}

			if (!result)
			{
				graphics::TextureAtlas atlas;
				auto id = atlas.get_id();

				auto pair = m_atlas.emplace(id, std::move(atlas));
				pair.first->second.add(file);

				m_batches[id] = {};
				m_batches[id].add_texture(pair.first->second.gl_texture());

				result = true;
			}

			return result;
		}

		void TextureBook::add(components::BatchSprite* sprite, components::Transform2D* transform)
		{
			m_batches[sprite->get_atlas_index()].add(sprite, transform);
		}

		void TextureBook::add_multi(std::span<std::string> files)
		{
			for (const auto& file : files)
			{
				add(file);
			}
		}

		void TextureBook::add_json(std::string_view file)
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

		void TextureBook::add_custom_region(const unsigned int index, std::string_view key, const math::Rect<float>& region)
		{
			m_atlas[index].add_custom_region(key, region);
		}

		std::optional<graphics::TextureInfo> TextureBook::search(std::string_view key)
		{
			for (auto& [id, atlas] : m_atlas)
			{
				if (atlas.contains(key))
				{
					return atlas.get_texture_info(key);
				}
			}

			return std::nullopt;
		}

		std::optional<graphics::TextureInfo> TextureBook::get(unsigned int index, std::string_view key)
		{
			return m_atlas[index].get_texture_info(key);
		}

		void TextureBook::buffer_spritebatch_data()
		{
			for (auto& [index, batch] : m_batches)
			{
				batch.buffer_data();
			}
		}

		void TextureBook::clear() noexcept
		{
			m_atlas.clear();
			m_batches.clear();
		}

		void TextureBook::clear_sprites() noexcept
		{
			m_batches.clear();
		}

		TextureBook::BatchMap& TextureBook::get_spritebatches() noexcept
		{
			return m_batches;
		}
	} // namespace res
} // namespace galaxy