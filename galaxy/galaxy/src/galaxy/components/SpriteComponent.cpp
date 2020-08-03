///
/// SpriteComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SpriteComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteComponent::SpriteComponent()
	    : m_sprite {}
	{
	}

	SpriteComponent::SpriteComponent(const nlohmann::json& json)
	    : m_sprite {}
	{
		std::string path = json.at("texture");
		m_sprite.load(path);

		const bool dynamic = json.at("is-dynamic-buffer");
		if (dynamic)
		{
			m_sprite.create<qs::BufferDynamic>();
		}
		else
		{
			m_sprite.create<qs::BufferStatic>();
		}

		m_sprite.set_anisotropy(json.at("ansio-filtering"));
		m_sprite.set_opacity(json.at("opacity"));
		m_sprite.set_pos(json.at("x"), json.at("y"));
		m_sprite.set_z_level(json.at("z-level"));
	}
} // namespace galaxy