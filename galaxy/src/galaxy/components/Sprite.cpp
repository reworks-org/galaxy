///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/resource/Textures.hpp"

#include "Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite::Sprite()
			: Serializable {}
			, m_texture {nullptr}
		//, m_mapped_vbo {nullptr}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
			: Serializable {}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite&& s)
			: Serializable {}
		{
			/*if (this->m_mapped_vbo != nullptr)
				{
					glUnmapNamedBuffer(this->m_vao.vbo().id());
				}*/

			this->m_texture  = s.m_texture;
			this->m_vao      = std::move(s.m_vao);
			this->m_tint     = std::move(s.m_tint);
			this->m_name     = std::move(s.m_name);
			this->m_vertices = std::move(s.m_vertices);
			// this->m_mapped_vbo = std::move(s.m_mapped_vbo);

			s.m_texture = nullptr;
			// s.m_mapped_vbo = nullptr;
		}

		Sprite& Sprite::operator=(Sprite&& s)
		{
			if (this != &s)
			{
				/*if (this->m_mapped_vbo != nullptr)
				{
					glUnmapNamedBuffer(this->m_vao.vbo().id());
				}*/

				this->m_texture  = s.m_texture;
				this->m_vao      = std::move(s.m_vao);
				this->m_tint     = std::move(s.m_tint);
				this->m_name     = std::move(s.m_name);
				this->m_vertices = std::move(s.m_vertices);
				// this->m_mapped_vbo = std::move(s.m_mapped_vbo);

				s.m_texture = nullptr;
				// s.m_mapped_vbo = nullptr;
			}

			return *this;
		}

		Sprite::~Sprite()
		{
			/*if (m_mapped_vbo != nullptr)
			{
				glUnmapNamedBuffer(m_vao.vbo().id());
			}*/
		}

		void Sprite::set_texture(const std::string& texture)
		{
			auto& cache = core::ServiceLocator<resource::Textures>::ref();
			auto  tex   = cache.get(texture);

			if (tex)
			{
				/*if (m_mapped_vbo != nullptr)
				{
					glUnmapNamedBuffer(m_vao.vbo().id());
				}*/

				m_name    = texture;
				m_texture = tex;

				m_vertices   = graphics::gen_quad_vertices(m_texture->width(), m_texture->height());
				auto indices = graphics::gen_default_indices();

				m_vao.buffer(m_vertices, indices);
				/*m_mapped_vbo = glMapNamedBufferRange(m_vao.vbo().id(),
					0,
					m_vertices.size() * sizeof(graphics::Vertex),
					GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT);*/
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::set_texture(const std::string& texture, const math::fRect& rect)
		{
			auto& cache = core::ServiceLocator<resource::Textures>::ref();
			auto  tex   = cache.get(texture);

			if (tex)
			{
				/*if (m_mapped_vbo != nullptr)
				{
					glUnmapNamedBuffer(m_vao.vbo().id());
				}*/

				m_name    = texture;
				m_texture = tex;

				m_vertices[0].m_pos.x    = 0.0f;
				m_vertices[0].m_pos.y    = 0.0f;
				m_vertices[0].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
				m_vertices[0].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

				m_vertices[1].m_pos.x    = m_texture->width();
				m_vertices[1].m_pos.y    = 0.0f;
				m_vertices[1].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
				m_vertices[1].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

				m_vertices[2].m_pos.x    = m_texture->width();
				m_vertices[2].m_pos.y    = m_texture->height();
				m_vertices[2].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
				m_vertices[2].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

				m_vertices[3].m_pos.x    = 0.0f;
				m_vertices[3].m_pos.y    = m_texture->height();
				m_vertices[3].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
				m_vertices[3].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

				auto indices = graphics::gen_default_indices();

				m_vao.buffer(m_vertices, indices);
				/*m_mapped_vbo = glMapNamedBufferRange(m_vao.vbo().id(),
					0,
					m_vertices.size() * sizeof(graphics::Vertex),
					GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_INVALIDATE_RANGE_BIT);*/
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::set_clip(const math::fRect& rect)
		{
			// if (m_mapped_vbo)
			//{
			m_vertices[0].m_pos.x    = 0.0f;
			m_vertices[0].m_pos.y    = 0.0f;
			m_vertices[0].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
			m_vertices[0].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

			m_vertices[1].m_pos.x    = m_texture->width();
			m_vertices[1].m_pos.y    = 0.0f;
			m_vertices[1].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
			m_vertices[1].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

			m_vertices[2].m_pos.x    = m_texture->width();
			m_vertices[2].m_pos.y    = m_texture->height();
			m_vertices[2].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
			m_vertices[2].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

			m_vertices[3].m_pos.x    = 0.0f;
			m_vertices[3].m_pos.y    = m_texture->height();
			m_vertices[3].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
			m_vertices[3].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

			m_vao.sub_buffer(0, m_vertices);
			// std::memcpy(m_mapped_vbo, m_vertices.data(), m_vertices.size() * sizeof(graphics::Vertex));
			// glFlushMappedNamedBufferRange(m_vao.vbo().id(), 0, m_vertices.size() * sizeof(graphics::Vertex));
			//}
		}

		graphics::Texture2D* Sprite::get_texture()
		{
			return m_texture;
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_name;
			json["tint"]["r"]   = m_tint.r<std::uint8_t>();
			json["tint"]["g"]   = m_tint.g<std::uint8_t>();
			json["tint"]["b"]   = m_tint.b<std::uint8_t>();
			json["tint"]["a"]   = m_tint.a<std::uint8_t>();

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			const auto& tint = json.at("tint");
			m_tint.r(tint.at("r").get<std::uint8_t>());
			m_tint.g(tint.at("g").get<std::uint8_t>());
			m_tint.b(tint.at("b").get<std::uint8_t>());
			m_tint.a(tint.at("a").get<std::uint8_t>());

			set_texture(json.at("texture"));
		}
	} // namespace components
} // namespace galaxy
