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
			this->m_texture = s.m_texture;
			this->m_vao     = std::move(s.m_vao);
			this->m_tint    = std::move(s.m_tint);
			this->m_name    = std::move(s.m_name);

			s.m_texture = nullptr;
		}

		Sprite& Sprite::operator=(Sprite&& s)
		{
			if (this != &s)
			{
				this->m_texture = s.m_texture;
				this->m_vao     = std::move(s.m_vao);
				this->m_tint    = std::move(s.m_tint);
				this->m_name    = std::move(s.m_name);

				s.m_texture = nullptr;
			}

			return *this;
		}

		Sprite::~Sprite()
		{
		}

		void Sprite::set_texture(const std::string& texture)
		{
			auto& cache = core::ServiceLocator<resource::Textures>::ref();
			auto  tex   = cache.get(texture);

			if (tex)
			{
				m_name    = texture;
				m_texture = tex;

				auto vertices = graphics::gen_quad_vertices(m_texture->width(), m_texture->height());
				auto indices  = graphics::gen_default_indices();

				m_vao.buffer(vertices, indices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		void Sprite::set_texture(const std::string& texture, const math::iRect& rect)
		{
			auto& cache = core::ServiceLocator<resource::Textures>::ref();
			auto  tex   = cache.get(texture);

			if (tex)
			{
				m_name    = texture;
				m_texture = tex;

				// clang-format off
				std::array<graphics::Vertex, 4> vertices = 
				{
					graphics::Vertex {.m_pos = glm::vec2 {0.0f, 0.0f}},
					graphics::Vertex {.m_pos = glm::vec2 {m_texture->width(), 0.0f}},
					graphics::Vertex {.m_pos = glm::vec2 {m_texture->width(), m_texture->height()}},
					graphics::Vertex {.m_pos = glm::vec2 {0.0f, m_texture->height()}}
				};
				// clang-format on

				vertices[0].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
				vertices[0].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

				vertices[1].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
				vertices[1].m_texels.y = graphics::map_y_texel(rect.y, m_texture->height());

				vertices[2].m_texels.x = graphics::map_x_texel(rect.x + rect.width, m_texture->width());
				vertices[2].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

				vertices[3].m_texels.x = graphics::map_x_texel(rect.x, m_texture->width());
				vertices[3].m_texels.y = graphics::map_y_texel(rect.y + rect.height, m_texture->height());

				auto indices = graphics::gen_default_indices();

				m_vao.buffer(vertices, indices);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to query texture atlas for '{0}'.", texture);
			}
		}

		graphics::Texture2D* Sprite::get_texture()
		{
			return m_texture.get();
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
