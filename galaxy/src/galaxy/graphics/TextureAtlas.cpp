///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace graphics
	{
		TextureAtlas::TextureAtlas()
		{
			init();
		}

		TextureAtlas::TextureAtlas(std::span<std::string> files)
		{
			init();
			add(files);
		}

		TextureAtlas::TextureAtlas(TextureAtlas&& ta) noexcept
		{
			if (this != &ta)
			{
				this->m_max_bindings = ta.m_max_bindings;
				this->m_size         = ta.m_size;
				this->m_sheets       = std::move(ta.m_sheets);
				this->m_data         = std::move(ta.m_data);
				this->m_transform    = std::move(ta.m_transform);
				this->m_va           = std::move(ta.m_va);
			}
		}

		TextureAtlas& TextureAtlas::operator=(TextureAtlas&& ta) noexcept
		{
			if (this != &ta)
			{
				this->m_max_bindings = ta.m_max_bindings;
				this->m_size         = ta.m_size;
				this->m_sheets       = std::move(ta.m_sheets);
				this->m_data         = std::move(ta.m_data);
				this->m_transform    = std::move(ta.m_transform);
				this->m_va           = std::move(ta.m_va);
			}

			return *this;
		}

		TextureAtlas::~TextureAtlas()
		{
			clear();
		}

		void TextureAtlas::add(std::string_view file)
		{
			Texture texture;
			if (texture.load(file))
			{
				const auto path = std::filesystem::path(file);
				const auto name = path.stem().string();

				if (!m_data.contains(name))
				{
					TextureAtlas::Info info;
					std::optional<iRect> packed = std::nullopt;

					for (auto i = 0; i < m_sheets.size(); i++)
					{
						if (!m_sheets[i].m_sheet_created)
						{
							m_sheets[i].m_packer.init(m_size, m_size);
							m_sheets[i].m_render_texture.create(m_size, m_size);
							m_sheets[i].m_sheet_created = true;
						}

						packed = m_sheets[i].m_packer.pack(texture.get_width(), texture.get_height());
						if (packed.has_value())
						{
							info.m_index = i;
							break;
						}
					}

					if (packed.has_value())
					{
						auto& sheet = m_sheets[info.m_index];

						info.m_region = packed.value();
						sheet.m_render_texture.bind(false);

						// Update transform.
						m_transform.set_pos(info.m_region.m_x, info.m_region.m_y);

						// Redefine vertices.
						std::array<graphics::Vertex, 4> vertices;
						vertices[0].m_pos    = {0.0f, 0.0f};
						vertices[0].m_texels = {0.0f, 0.0f};

						vertices[1].m_pos    = {info.m_region.m_width, 0.0f};
						vertices[1].m_texels = {1.0f, 0.0f};

						vertices[2].m_pos    = {info.m_region.m_width, info.m_region.m_height};
						vertices[2].m_texels = {1.0f, 1.0f};

						vertices[3].m_pos    = {0.0f, info.m_region.m_height};
						vertices[3].m_texels = {0.0f, 1.0f};

						m_va.sub_buffer(0, vertices);
						Renderer::draw_texture_to_target(sheet.m_render_texture, texture, m_va, m_transform);
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Unable to find room to pack '{0}' into a texture atlas.", file);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to add duplicate texture to texture atlas.");
				}

				glBindTexture(GL_TEXTURE_2D, 0);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
		}

		void TextureAtlas::add(std::span<std::string> files)
		{
			for (const auto& file : files)
			{
				add(file);
			}
		}

		void TextureAtlas::save()
		{
			for (auto i = 0; i < m_sheets.size(); i++)
			{
				m_sheets[i].m_render_texture.save(std::format("dump/sheet{0}.png", i));
			}
		}

		void TextureAtlas::bind() noexcept
		{
			for (auto i = 0; i < m_sheets.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_sheets[i].m_render_texture.get_texture());
			}
		}

		void TextureAtlas::unbind() noexcept
		{
			for (auto i = m_sheets.size(); i > 0; i--)
			{
				glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + i));
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		void TextureAtlas::clear()
		{
			m_sheets.clear();
			m_data.clear();
			m_transform.reset();

			init();
		}

		bool TextureAtlas::contains(const std::string& key) noexcept
		{
			return m_data.contains(key);
		}

		meta::OptionalRef<TextureAtlas::Info> TextureAtlas::query(const std::string& key) noexcept
		{
			if (contains(key))
			{
				return std::make_optional(std::ref(m_data[key]));
			}
			else
			{
				return std::nullopt;
			}
		}

		nlohmann::json TextureAtlas::serialize()
		{
			auto json = nlohmann::json::array();
			for (const auto& [key, info] : m_data)
			{
				json.push_back(key + ".png");
			}

			return json;
		}

		void TextureAtlas::deserialize(const nlohmann::json& json)
		{
			clear();
			for (const auto& path : json)
			{
				add(path.get<std::string>());
			}
		}

		void TextureAtlas::init()
		{
			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_max_bindings);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);

			// Hard limit size to support older hardware.
			m_max_bindings = std::min(m_max_bindings, 32);
			m_size         = std::min(m_size, 4096);

			m_sheets.resize(m_max_bindings);

			auto vb = std::make_unique<VertexBuffer>();
			auto ib = std::make_unique<IndexBuffer>();

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {0.0f, 0.0f};

			vertices[1].m_pos    = {1.0f, 0.0f};
			vertices[1].m_texels = {1.0f, 0.0f};

			vertices[2].m_pos    = {1.0f, 1.0f};
			vertices[2].m_texels = {1.0f, 1.0f};

			vertices[3].m_pos    = {0.0f, 1.0f};
			vertices[3].m_texels = {0.0f, 1.0f};

			std::array<unsigned int, 6> indices = {0, 1, 3, 1, 2, 3};

			vb->create(vertices, StorageFlag::DYNAMIC_DRAW);
			ib->create(indices, StorageFlag::STATIC_DRAW);

			m_va.create(vb, ib);
		}
	} // namespace graphics
} // namespace galaxy