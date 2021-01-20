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
	namespace graphics
	{
		Sprite::Sprite()
		    : VertexData {}, Texture {}, Transform {}
		{
		}

		Sprite::Sprite(const nlohmann::json& json)
		    : VertexData {}, Texture {}, Transform {}
		{
			std::string tex = json.at("texture");
			load(tex);

			const bool dynamic = json.at("is-dynamic-buffer");
			if (dynamic)
			{
				create<BufferDynamic>();
			}
			else
			{
				create<BufferStatic>();
			}

			set_anisotropy(json.at("ansio-filtering"));
			set_opacity(json.at("opacity"));
			set_pos(json.at("x"), json.at("y"));
			set_z_level(json.at("z-level"));
		}

		void Sprite::bind()
		{
			m_va.bind();
			glBindTexture(GL_TEXTURE_2D, m_texture);
		}

		void Sprite::unbind()
		{
			m_va.unbind();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy