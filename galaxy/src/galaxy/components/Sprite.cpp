///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite::Sprite() noexcept
		    : Serializable {this}, VertexData {}, Texture {}, m_opacity {1.0f}, m_texture_str {""}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
		    : Serializable {this}, VertexData {}, Texture {}, m_opacity {1.0f}, m_texture_str {""}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite&& s) noexcept
		    : Serializable {this}, VertexData {std::move(s)}, Texture {std::move(s)}
		{
			this->m_opacity     = s.m_opacity;
			this->m_texture_str = s.m_texture_str;
			this->m_vertexs     = std::move(s.m_vertexs);
		}

		Sprite& Sprite::operator=(Sprite&& s) noexcept
		{
			if (this != &s)
			{
				VertexData::operator=(std::move(s));

				Texture::operator=(std::move(s));

				this->m_opacity     = s.m_opacity;
				this->m_texture_str = s.m_texture_str;
				this->m_vertexs     = std::move(s.m_vertexs);
			}

			return *this;
		}

		void Sprite::create(const float tex_x, const float tex_y)
		{
			m_vertexs.clear();
			m_vertexs.push_back({0.0f, 0.0f, tex_x, tex_y});
			m_vertexs.push_back({0.0f + m_width, 0.0f, tex_x + m_width, tex_y});
			m_vertexs.push_back({0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height});
			m_vertexs.push_back({0.0f, 0.0f + m_height, tex_x, tex_y + m_height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(m_vertexs);
			m_ib.create(ib_arr);

			m_layout.add<graphics::SpriteVertex, meta::VAPosition>(2);
			m_layout.add<graphics::SpriteVertex, meta::VATexel>(2);

			m_va.create<graphics::SpriteVertex>(m_vb, m_ib, m_layout);
		}

		void Sprite::create_clipped(const float width, const float height)
		{
			m_vertexs.clear();
			m_vertexs.push_back({0.0f, 0.0f, 0.0f, 0.0f});
			m_vertexs.push_back({0.0f + m_width, 0.0f, 0.0f + width, 0.0f});
			m_vertexs.push_back({0.0f + m_width, 0.0f + m_height, 0.0f + width, 0.0f + height});
			m_vertexs.push_back({0.0f, 0.0f + m_height, 0.0f, 0.0f + height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(m_vertexs);
			m_ib.create(ib_arr);

			m_layout.add<graphics::SpriteVertex, meta::VAPosition>(2);
			m_layout.add<graphics::SpriteVertex, meta::VATexel>(2);

			m_va.create<graphics::SpriteVertex>(m_vb, m_ib, m_layout);
		}

		void Sprite::bind() noexcept
		{
			m_va.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Sprite::unbind() noexcept
		{
			m_va.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Sprite::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		const float Sprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const std::vector<graphics::SpriteVertex>& Sprite::get_vertexs() const noexcept
		{
			return m_vertexs;
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_texture_str;
			json["opacity"]     = m_opacity;

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			m_texture_str = json.at("texture");
			load(m_texture_str);
			create();

			set_opacity(json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy