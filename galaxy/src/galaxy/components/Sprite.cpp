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
		    : Texture {}, Serializable {this}, m_opacity {255}, m_depth {0}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
		    : Texture {}, Serializable {this}, m_opacity {255}, m_depth {0}
		{
			deserialize(json);
		}

		Sprite::Sprite(Sprite&& s) noexcept
		    : Texture {std::move(s)}, Serializable {this}
		{
			this->m_vao     = std::move(s.m_vao);
			this->m_opacity = s.m_opacity;
			this->m_depth   = s.m_depth;
		}

		Sprite& Sprite::operator=(Sprite&& s) noexcept
		{
			if (this != &s)
			{
				Texture::operator=(std::move(s));

				this->m_vao     = std::move(s.m_vao);
				this->m_opacity = s.m_opacity;
				this->m_depth   = s.m_depth;
			}

			return *this;
		}

		Sprite::~Sprite() noexcept
		{
		}

		void Sprite::create(const int depth, const float tex_x, const float tex_y)
		{
			m_depth = depth;

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {tex_x, tex_y};
			vertices[0].set_depth(m_depth);

			vertices[1].m_pos    = {0.0f + m_width, 0.0f};
			vertices[1].m_texels = {tex_x + m_width, tex_y};
			vertices[1].set_depth(m_depth);

			vertices[2].m_pos    = {0.0f + m_width, 0.0f + m_height};
			vertices[2].m_texels = {tex_x + m_width, tex_y + m_height};
			vertices[2].set_depth(m_depth);

			vertices[3].m_pos    = {0.0f, 0.0f + m_height};
			vertices[3].m_texels = {tex_x, tex_y + m_height};
			vertices[3].set_depth(m_depth);

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;
			graphics::VertexLayout layout;

			vbo.create(vertices, false);
			ibo.create(indices, true);

			layout.add<graphics::VertexAttributes::POSITION>(2);
			layout.add<graphics::VertexAttributes::TEXEL>(2);
			layout.add<graphics::VertexAttributes::COLOUR>(4);
			layout.add<graphics::VertexAttributes::DEPTH>(1);

			m_vao.create(vbo, ibo, layout);
		}

		void Sprite::create_clipped(const int depth, const float width, const float height)
		{
			m_depth = depth;

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {0.0f, 0.0f};
			vertices[0].set_depth(m_depth);

			vertices[1].m_pos    = {0.0f + width, 0.0f};
			vertices[1].m_texels = {0.0f + width, 0.0f};
			vertices[1].set_depth(m_depth);

			vertices[2].m_pos    = {0.0f + width, 0.0f + height};
			vertices[2].m_texels = {0.0f + width, 0.0f + height};
			vertices[2].set_depth(m_depth);

			vertices[3].m_pos    = {0.0f, 0.0f + height};
			vertices[3].m_texels = {0.0f, 0.0f + height};
			vertices[3].set_depth(m_depth);

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;
			graphics::VertexLayout layout;

			vbo.create(vertices, false);
			ibo.create(indices, true);

			layout.add<graphics::VertexAttributes::POSITION>(2);
			layout.add<graphics::VertexAttributes::TEXEL>(2);
			layout.add<graphics::VertexAttributes::COLOUR>(4);
			layout.add<graphics::VertexAttributes::DEPTH>(1);

			m_vao.create(vbo, ibo, layout);
		}

		void Sprite::create_clipped(const int depth, const float x, const float y, const float width, const float height)
		{
			m_depth = depth;

			std::array<graphics::Vertex, 4> vertices;
			vertices[0].m_pos    = {0.0f, 0.0f};
			vertices[0].m_texels = {x, y};
			vertices[0].set_depth(m_depth);

			vertices[1].m_pos    = {0.0f + width, 0.0f};
			vertices[1].m_texels = {x + width, y};
			vertices[1].set_depth(m_depth);

			vertices[2].m_pos    = {0.0f + width, 0.0f + height};
			vertices[2].m_texels = {x + width, y + height};
			vertices[2].set_depth(m_depth);

			vertices[3].m_pos    = {0.0f, 0.0f + height};
			vertices[3].m_texels = {x, y + height};
			vertices[3].set_depth(m_depth);

			std::array<unsigned int, 6> indices =
			    {0, 1, 3, 1, 2, 3};

			graphics::VertexBuffer vbo;
			graphics::IndexBuffer ibo;
			graphics::VertexLayout layout;

			vbo.create(vertices, false);
			ibo.create(indices, true);

			layout.add<graphics::VertexAttributes::POSITION>(2);
			layout.add<graphics::VertexAttributes::TEXEL>(2);
			layout.add<graphics::VertexAttributes::COLOUR>(4);
			layout.add<graphics::VertexAttributes::DEPTH>(1);

			m_vao.create(vbo, ibo, layout);
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

		const int Sprite::get_depth() const noexcept
		{
			return m_depth;
		}

		const unsigned int Sprite::count() const noexcept
		{
			return m_vao.count();
		}

		nlohmann::json Sprite::serialize()
		{
			nlohmann::json json = "{}"_json;
			json["texture"]     = m_path;
			json["depth"]       = m_depth;
			json["opacity"]     = m_opacity;

			return json;
		}

		void Sprite::deserialize(const nlohmann::json& json)
		{
			const std::string& texture = json.at("texture");
			load(texture);
			create(json.at("depth"));

			set_opacity(json.at("opacity"));
		}
	} // namespace components
} // namespace galaxy