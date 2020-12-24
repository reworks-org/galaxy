///
/// Sprite.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include "Sprite.hpp"

///
/// Core namespace.
///
namespace qs
{
	Sprite::Sprite()
	    : VertexData {}, Texture {}, Transform {}
	{
	}

	Sprite::Sprite(const nlohmann::json& json)
	    : m_sprite {}
	{
		std::string tex = json.at("texture");
		auto path       = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + tex;
		m_sprite.load(path);

		const bool dynamic = json.at("is-dynamic-buffer");
		if (dynamic)
		{
			m_sprite.create<graphics::BufferDynamic>();
		}
		else
		{
			m_sprite.create<graphics::BufferStatic>();
		}

		m_sprite.set_anisotropy(json.at("ansio-filtering"));
		m_sprite.set_opacity(json.at("opacity"));
		m_sprite.set_pos(json.at("x"), json.at("y"));
		m_sprite.set_z_level(json.at("z-level"));
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
} // namespace qs