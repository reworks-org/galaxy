///
/// Sprite.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Sprite.hpp"

namespace galaxy
{
	namespace components
	{
		Sprite::Sprite() noexcept
		    : Serializable {this}, m_opacity {255}, m_layer {""}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
		    : Serializable {this}, m_opacity {255}, m_layer {""}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite&& s) noexcept
		    : Serializable {this}, Texture {std::move(s)}
		{
			this->m_vao     = std::move(s.m_vao);
			this->m_opacity = s.m_opacity;
			this->m_layer   = std::move(s.m_layer);
		}

		Sprite& Sprite::operator=(Sprite&& s) noexcept
		{
			if (this != &s)
			{
				Texture::operator=(std::move(s));

				this->m_vao     = std::move(s.m_vao);
				this->m_opacity = s.m_opacity;
				this->m_layer   = std::move(s.m_layer);
			}

			return *this;
		}

		Sprite::~Sprite() noexcept
		{
		}

		void Sprite::create(std::string_view layer, const float tex_x, const float tex_y)
		{
			m_layer = static_cast<std::string>(layer);

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {tex_x, tex_y};

			vertices[1].m_pos    = {0.0f + m_width, 0.0f};
			vertices[1].m_texels = {tex_x + m_width, tex_y};

			vertices[2].m_pos    = {0.0f + m_width, 0.0f + m_height};
			vertices[2].m_texels = {tex_x + m_width, tex_y + m_height};

			vertices[3].m_pos    = {0.0f, 0.0f + m_height};
			vertices[3].m_texels = {tex_x, tex_y + m_height};

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;

			vbo.create(vertices, false);
			ibo.create(indices, true);

			m_vao.create(vbo, ibo);
		}

		void Sprite::create_clipped(std::string_view layer, const float width, const float height)
		{
			m_layer = static_cast<std::string>(layer);

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {0.0f, 0.0f};

			vertices[1].m_pos    = {0.0f + width, 0.0f};
			vertices[1].m_texels = {0.0f + width, 0.0f};

			vertices[2].m_pos    = {0.0f + width, 0.0f + height};
			vertices[2].m_texels = {0.0f + width, 0.0f + height};

			vertices[3].m_pos    = {0.0f, 0.0f + height};
			vertices[3].m_texels = {0.0f, 0.0f + height};

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;

			vbo.create(vertices, false);
			ibo.create(indices, true);
			m_vao.create(vbo, ibo);
		}

		void Sprite::create_clipped(std::string_view layer, const float x, const float y, const float width, const float height)
		{
			m_layer = static_cast<std::string>(layer);

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {x, y};

			vertices[1].m_pos    = {0.0f + width, 0.0f};
			vertices[1].m_texels = {x + width, y};

			vertices[2].m_pos    = {0.0f + width, 0.0f + height};
			vertices[2].m_texels = {x + width, y + height};

			vertices[3].m_pos    = {0.0f, 0.0f + height};
			vertices[3].m_texels = {x, y + height};

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;

			vbo.create(vertices, false);
			ibo.create(indices, true);
			m_vao.create(vbo, ibo);
		}

		void Sprite::bind() noexcept
		{
			m_vao.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Sprite::unbind() noexcept
		{
			m_vao.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Sprite::set_opacity(const std::uint8_t opacity) noexcept
		{
			m_opacity = std::clamp<std::uint8_t>(opacity, 0, 255);
		}

		const std::uint8_t Sprite::get_opacity() const noexcept
		{
			return m_opacity;
		}

		const std::string& Sprite::get_layer() const noexcept
		{
			return m_layer;
		}

		const unsigned int Sprite::vao() const noexcept
		{
			return m_vao.id();
		}

		const int Sprite::index_count() const noexcept
		{
			return m_vao.index_count();
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;

			json["texture"] = m_path;
			json["layer"]   = m_layer;
			json["opacity"] = m_opacity;

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			load(json.at("texture").get<std::string>());
			create(json.at("layer"));

			set_opacity(json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy