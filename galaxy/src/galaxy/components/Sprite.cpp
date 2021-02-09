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
		    : VertexData {}, Texture {}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
		    : VertexData {}, Texture {}
		{
			load(json.at("texture"));
			create();

			set_opacity(json.at("opacity"));
		}

		Sprite::Sprite(Sprite&& s) noexcept
		    : VertexData {std::move(s)}, Texture {std::move(s)}
		{
		}

		Sprite& Sprite::operator=(Sprite&& s) noexcept
		{
			if (this != &s)
			{
				VertexData::operator=(std::move(s));

				Texture::operator=(std::move(s));
			}

			return *this;
		}

		void Sprite::stretch(const float width, const float height) noexcept
		{
			m_width  = width;
			m_height = height;
		}

		void Sprite::create(const float tex_x, const float tex_y)
		{
			std::vector<graphics::SpriteVertex> vb_arr;
			vb_arr.push_back({0.0f, 0.0f, tex_x, tex_y});
			vb_arr.push_back({0.0f + m_width, 0.0f, tex_x + m_width, tex_y});
			vb_arr.push_back({0.0f + m_width, 0.0f + m_height, tex_x + m_width, tex_y + m_height});
			vb_arr.push_back({0.0f, 0.0f + m_height, tex_x, tex_y + m_height});

			std::array<unsigned int, 6> ib_arr = {0, 1, 3, 1, 2, 3};

			m_vb.create<graphics::SpriteVertex>(vb_arr);
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
	} // namespace components
} // namespace galaxy