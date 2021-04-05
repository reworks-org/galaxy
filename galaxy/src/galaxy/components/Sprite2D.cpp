///
/// Sprite2D.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "Sprite2D.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite2D::Sprite2D() noexcept
		    : Serializable {this}, VertexData {}, Texture {}, m_opacity {1.0f}, m_texture_str {""}
		{
		}

		Sprite2D::Sprite2D(const nlohmann::json& json)
		    : Serializable {this}, VertexData {}, Texture {}, m_opacity {1.0f}, m_texture_str {""}
		{
			deserialize(json);
		}

		Sprite2D::Sprite2D(Sprite2D&& s) noexcept
		    : Serializable {this}, VertexData {std::move(s)}, Texture {std::move(s)}
		{
			this->m_opacity     = s.m_opacity;
			this->m_texture_str = s.m_texture_str;
			this->m_vertexs     = std::move(s.m_vertexs);
		}

		Sprite2D& Sprite2D::operator=(Sprite2D&& s) noexcept
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

		void Sprite2D::create(const float tex_x, const float tex_y)
		{
			m_vertexs.clear();
			m_vertexs.push_back({0.0f, 0.0f, tex_x, tex_y});
			m_vertexs.push_back({0.0f + m_width, 0.0f, tex_x + m_width, tex_y});
			m_vertexs.push_back({0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height});
			m_vertexs.push_back({0.0f, 0.0f + m_height, tex_x, tex_y + m_height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(m_vertexs);
			m_ib.create(ib_arr);

			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::POSITION>(2);
			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::TEXEL>(2);

			m_va.create<graphics::SpriteVertex>(m_vb, m_ib, m_layout);
		}

		void Sprite2D::create_clipped(const float width, const float height)
		{
			m_vertexs.clear();
			m_vertexs.push_back({0.0f, 0.0f, 0.0f, 0.0f});
			m_vertexs.push_back({0.0f + width, 0.0f, 0.0f + width, 0.0f});
			m_vertexs.push_back({0.0f + width, 0.0f + height, 0.0f + width, 0.0f + height});
			m_vertexs.push_back({0.0f, 0.0f + height, 0.0f, 0.0f + height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(m_vertexs);
			m_ib.create(ib_arr);

			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::POSITION>(2);
			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::TEXEL>(2);

			m_va.create<graphics::SpriteVertex>(m_vb, m_ib, m_layout);
		}

		void Sprite2D::create_clipped(const float x, const float y, const float width, const float height)
		{
			m_vertexs.clear();
			m_vertexs.push_back({0.0f, 0.0f, x, y});
			m_vertexs.push_back({0.0f + width, 0.0f, x + width, y});
			m_vertexs.push_back({0.0f + width, 0.0f + height, x + width, y + height});
			m_vertexs.push_back({0.0f, 0.0f + height, x, y + height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(m_vertexs);
			m_ib.create(ib_arr);

			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::POSITION>(2);
			m_layout.add<graphics::SpriteVertex, graphics::VertexAttributes::TEXEL>(2);

			m_va.create<graphics::SpriteVertex>(m_vb, m_ib, m_layout);
		}

		void Sprite2D::bind() noexcept
		{
			m_va.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Sprite2D::unbind() noexcept
		{
			m_va.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Sprite2D::set_opacity(const float opacity) noexcept
		{
			m_opacity = std::clamp(opacity, 0.0f, 1.0f);
		}

		const float Sprite2D::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const std::vector<graphics::SpriteVertex>& Sprite2D::get_vertexs() const noexcept
		{
			return m_vertexs;
		}

		nlohmann::json Sprite2D::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_texture_str;
			json["opacity"]     = m_opacity;

			return json;
		}

		void Sprite2D::deserialize(const nlohmann::json& json)
		{
			m_texture_str = json.at("texture");
			load(m_texture_str);
			create();

			set_opacity(json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy