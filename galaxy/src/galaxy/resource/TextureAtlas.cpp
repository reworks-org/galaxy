///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <glad/glad.h>

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace resource
	{
		TextureAtlas::TextureAtlas()
			: m_max_bindings {0}
			, m_size {0}
		{
			init();
		}

		TextureAtlas::~TextureAtlas()
		{
			clear();
		}

		void TextureAtlas::add(const std::string& file)
		{
			auto& fs   = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data = fs.read_binary(file);
			if (!data.empty())
			{
				graphics::Texture texture;
				texture.load_mem(data);

				const auto path = std::filesystem::path(file);
				const auto name = path.filename().string();

				if (!m_data.contains(name))
				{
					m_data[name] = {};

					std::optional<math::iRect> packed = std::nullopt;
					for (auto i = 0; i < m_sheets.size(); i++)
					{
						if (!m_sheets[i])
						{
							m_sheets[i] = std::make_unique<Sheet>();
							m_sheets[i]->m_packer.init(m_size, m_size);
							m_sheets[i]->m_render_texture.create(m_size, m_size);
						}

						packed = m_sheets[i]->m_packer.pack(texture.get_width(), texture.get_height());
						if (packed.has_value())
						{
							m_data[name].m_index = i;
							break;
						}
					}

					if (packed.has_value())
					{
						auto& sheet    = m_sheets[m_data[name].m_index];
						auto& renderer = core::ServiceLocator<graphics::Renderer>::ref();

						m_data[name].m_handle = sheet->m_render_texture.get_texture();
						m_data[name].m_region = packed.value();

						// Convert to texel coords.
						const auto sw = sheet->m_render_texture.get_width();
						const auto sh = sheet->m_render_texture.get_height();

						m_data[name].m_sheet_width  = sw;
						m_data[name].m_sheet_height = sh;

						m_data[name].m_texel_region.m_ul_texels.x = map_x_texel(m_data[name].m_region.x, sw);
						m_data[name].m_texel_region.m_ul_texels.y = map_y_texel(m_data[name].m_region.y, sh);

						m_data[name].m_texel_region.m_ur_texels.x = map_x_texel(m_data[name].m_region.x + m_data[name].m_region.width, sw);
						m_data[name].m_texel_region.m_ur_texels.y = map_y_texel(m_data[name].m_region.y, sh);

						m_data[name].m_texel_region.m_br_texels.x = map_x_texel(m_data[name].m_region.x + m_data[name].m_region.width, sw);
						m_data[name].m_texel_region.m_br_texels.y = map_y_texel(m_data[name].m_region.y + m_data[name].m_region.height, sh);

						m_data[name].m_texel_region.m_bl_texels.x = map_x_texel(m_data[name].m_region.x, sw);
						m_data[name].m_texel_region.m_bl_texels.y = map_y_texel(m_data[name].m_region.y + m_data[name].m_region.height, sh);

						// Update transform.
						m_transform.set_pos(static_cast<float>(m_data[name].m_region.x), static_cast<float>(m_data[name].m_region.y));

						// Redefine vertices.
						std::array<graphics::Vertex, 4> vertices;
						vertices[0].m_pos    = {0.0f, 0.0f};
						vertices[0].m_texels = {0.0f, 0.0f};

						vertices[1].m_pos    = {m_data[name].m_region.width, 0.0f};
						vertices[1].m_texels = {1.0f, 0.0f};

						vertices[2].m_pos    = {m_data[name].m_region.width, m_data[name].m_region.height};
						vertices[2].m_texels = {1.0f, 1.0f};

						vertices[3].m_pos    = {0.0f, m_data[name].m_region.height};
						vertices[3].m_texels = {0.0f, 1.0f};

						m_vao.sub_buffer(0, vertices);

						sheet->m_render_texture.bind(false);
						renderer.draw_texture_to_target(sheet->m_render_texture, texture, m_vao, m_transform);
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read '{0}' from vfs.", file);
			}
		}

		void TextureAtlas::add_from_vfs()
		{
			clear();

			for (const auto& file : core::ServiceLocator<fs::VirtualFileSystem>::ref().list(GALAXY_ATLAS_DIR))
			{
				add(file);
			}
		}

		void TextureAtlas::save()
		{
			for (auto i = 0; i < m_sheets.size(); i++)
			{
				if (m_sheets[i] != nullptr)
				{
					m_sheets[i]->m_render_texture.save(std::format("dump/sheet{0}.png", i));
				}
			}
		}

		void TextureAtlas::clear()
		{
			m_sheets.clear();
			m_data.clear();
			m_transform.reset();

			init();
		}

		bool TextureAtlas::contains(const std::string& key)
		{
			return m_data.contains(key);
		}

		meta::optional_ref<TextureAtlas::Info> TextureAtlas::query(const std::string& key)
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

		meta::vector<std::string> TextureAtlas::keys()
		{
			meta::vector<std::string> keys;
			keys.reserve(m_data.size());

			for (const auto& [key, _] : m_data)
			{
				keys.emplace_back(key);
			}

			return keys;
		}

		void TextureAtlas::init()
		{
			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_max_bindings);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);

			auto& config    = core::ServiceLocator<core::Config>::ref();
			auto  user_size = config.get<int>("texture_atlas_size", "graphics");

			if (user_size >= m_size)
			{
				user_size = m_size;
			}

			if (m_size != 1024 && m_size != 2048 && m_size != 4096 && m_size != 8192)
			{
				m_size = 4096;
			}

			// Hard limit bindings count.
			m_max_bindings = std::min(m_max_bindings, 32);

			m_sheets.resize(m_max_bindings);

			auto vertices = graphics::Vertex::gen_quad_vertices(1, 1);
			m_vao.create(vertices, graphics::StorageFlag::DYNAMIC_DRAW, graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);
		}
	} // namespace resource
} // namespace galaxy
