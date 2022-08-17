///
/// TextureAtlas.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <format>

#include <glad/glad.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/Renderer.hpp"

#include "TextureAtlas.hpp"

namespace galaxy
{
	namespace resource
	{
		TextureAtlas::TextureAtlas()
			: m_folder {""}
			, m_max_bindings {0}
			, m_size {0}
		{
			init();
		}

		TextureAtlas::~TextureAtlas()
		{
			clear();
		}

		void TextureAtlas::add_file(std::string_view file)
		{
			graphics::Texture texture;
			if (texture.load(file))
			{
				const auto path = std::filesystem::path(file);
				const auto name = path.stem().string();

				if (!m_data.contains(name))
				{
					TextureAtlas::Info info;
					std::optional<graphics::iRect> packed = std::nullopt;

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
						m_transform.set_pos(static_cast<float>(info.m_region.m_x), static_cast<float>(info.m_region.m_y));

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
						graphics::Renderer::draw_texture_to_target(sheet.m_render_texture, texture, m_va, m_transform);
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

		void TextureAtlas::add_folder(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					add_file(file);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to load any textures from '{0}'.", m_folder);
			}
		}

		void TextureAtlas::reload()
		{
			if (!m_folder.empty())
			{
				clear();
				add_folder(m_folder);
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

		void TextureAtlas::init()
		{
			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &m_max_bindings);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &m_size);

			// Hard limit size to support older hardware.
			m_max_bindings = std::min(m_max_bindings, 32);
			m_size         = std::min(m_size, 4096);

			m_sheets.resize(m_max_bindings);

			auto vb = std::make_unique<graphics::VertexBuffer>();
			auto ib = std::make_unique<graphics::IndexBuffer>();

			auto array = graphics::Vertex::gen_quad_vertices(1, 1);
			vb->create(array, graphics::StorageFlag::DYNAMIC_DRAW);
			ib->const_create(graphics::Vertex::get_default_indices(), graphics::StorageFlag::STATIC_DRAW);

			m_va.create(vb, ib);
		}
	} // namespace resource
} // namespace galaxy