///
/// SpriteBatch.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "SpriteBatch.hpp"

constexpr const auto max_quads = 100000;

namespace galaxy
{
	namespace graphics
	{
		SpriteBatch::SpriteBatch() noexcept
		    : m_texture {0}, m_width {0}, m_height {0}
		{
			VertexBuffer vbo;
			IndexBuffer ibo;
			VertexLayout layout;

			vbo.create({}, false, max_quads * 4);

			// Free memory immediately.
			{
				std::vector<unsigned int> is;
				is.reserve(max_quads * 6);

				unsigned int increment = 0;
				for (unsigned int counter = 0; counter < max_quads; counter++)
				{
					is.push_back(0 + increment);
					is.push_back(1 + increment);
					is.push_back(3 + increment);
					is.push_back(1 + increment);
					is.push_back(2 + increment);
					is.push_back(3 + increment);

					increment += 4;
				}

				ibo.create(is, true);
			}

			layout.add<VertexAttributes::POSITION>(2);
			layout.add<VertexAttributes::TEXEL>(2);
			layout.add<VertexAttributes::COLOUR>(4);
			layout.add<VertexAttributes::DEPTH>(1);

			m_vao.create(vbo, ibo, layout);
			m_vertices.reserve(max_quads * 4);
		}

		SpriteBatch::SpriteBatch(SpriteBatch&& sb) noexcept
		{
			this->m_vao      = std::move(sb.m_vao);
			this->m_vertices = std::move(sb.m_vertices);
			this->m_texture  = sb.m_texture;
			this->m_width    = sb.m_width;
			this->m_height   = sb.m_height;

			sb.m_vertices.clear();
			sb.m_texture = 0;
			sb.m_width   = 0;
			sb.m_height  = 0;
		}

		SpriteBatch& SpriteBatch::operator=(SpriteBatch&& sb) noexcept
		{
			if (this != &sb)
			{
				this->m_vao      = std::move(sb.m_vao);
				this->m_vertices = std::move(sb.m_vertices);
				this->m_texture  = sb.m_texture;
				this->m_width    = sb.m_width;
				this->m_height   = sb.m_height;

				sb.m_vertices.clear();
				sb.m_texture = 0;
				sb.m_width   = 0;
				sb.m_height  = 0;
			}

			return *this;
		}

		SpriteBatch::~SpriteBatch() noexcept
		{
			m_vertices.clear();
			m_texture = 0;
			m_width   = 0;
			m_height  = 0;
		}

		void SpriteBatch::add(components::BatchSprite* sprite, components::Transform2D* transform)
		{
			if ((m_vertices.size() * 4) > (max_quads * 4))
			{
				GALAXY_LOG(GALAXY_ERROR, "Too many quads in batch. Sprite not added. Max is {0}.", max_quads);
				return;
			}
			else
			{
				if (!sprite || !transform)
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to add nullptr to spritebatch.");
				}
				else
				{
					Vertex vertex;

					auto result     = (transform->get_transform() * glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f});
					vertex.m_pos    = {result.x, result.y};
					vertex.m_texels = {sprite->get_region().m_x, sprite->get_region().m_y};
					vertex.set_colour({0, 0, 0, sprite->get_opacity()});
					vertex.set_depth(sprite->get_depth());

					m_vertices.emplace_back(vertex);

					result          = (transform->get_transform() * glm::vec4 {0.0f + sprite->get_region().m_width, 0.0f, 0.0f, 1.0f});
					vertex.m_pos    = {result.x, result.y};
					vertex.m_texels = {sprite->get_region().m_x + sprite->get_region().m_width, sprite->get_region().m_y};
					vertex.set_colour({0, 0, 0, sprite->get_opacity()});
					vertex.set_depth(sprite->get_depth());

					m_vertices.emplace_back(vertex);

					result          = (transform->get_transform() * glm::vec4 {0.0f + sprite->get_region().m_width, 0.0f + sprite->get_region().m_height, 0.0f, 1.0f});
					vertex.m_pos    = {result.x, result.y};
					vertex.m_texels = {sprite->get_region().m_x + sprite->get_region().m_width, sprite->get_region().m_y + sprite->get_region().m_height};
					vertex.set_colour({0, 0, 0, sprite->get_opacity()});
					vertex.set_depth(sprite->get_depth());

					m_vertices.emplace_back(vertex);

					result          = (transform->get_transform() * glm::vec4 {0.0f, 0.0f + sprite->get_region().m_height, 0.0f, 1.0f});
					vertex.m_pos    = {result.x, result.y};
					vertex.m_texels = {sprite->get_region().m_x, sprite->get_region().m_y + sprite->get_region().m_height};
					vertex.set_colour({0, 0, 0, sprite->get_opacity()});
					vertex.set_depth(sprite->get_depth());

					m_vertices.emplace_back(vertex);
				}
			}
		}

		void SpriteBatch::add_texture(const unsigned int texture)
		{
			m_texture = texture;

			glGetTextureLevelParameteriv(m_texture, 0, GL_TEXTURE_WIDTH, &m_width);
			glGetTextureLevelParameteriv(m_texture, 0, GL_TEXTURE_HEIGHT, &m_height);
		}

		void SpriteBatch::buffer_data()
		{
			glNamedBufferData(m_vao.vbo(), sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_DYNAMIC_DRAW);
		}

		void SpriteBatch::bind() noexcept
		{
			m_vao.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void SpriteBatch::unbind() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			m_vao.unbind();
		}

		void SpriteBatch::clear() noexcept
		{
			m_vertices.clear();
		}

		const int SpriteBatch::get_width() const noexcept
		{
			return m_width;
		}

		const int SpriteBatch::get_height() const noexcept
		{
			return m_height;
		}

		const unsigned int SpriteBatch::count() const noexcept
		{
			// Prevents zero division.
			if (m_vertices.size() == 0)
			{
				return 0;
			}

			// Six indicies per quad. Number of quads is total vertexs / 4.
			// The size here is always a multiple of 4 so no "uneven" division takes place.
			return 6 * (m_vertices.size() / 4);
		}
	} // namespace graphics
} // namespace galaxy