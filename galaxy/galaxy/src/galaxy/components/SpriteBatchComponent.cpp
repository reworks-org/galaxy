///
/// SpriteBatchComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "SpriteBatchComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	SpriteBatchComponent::SpriteBatchComponent()
	    : m_bs {}
	{
	}

	SpriteBatchComponent::SpriteBatchComponent(const nlohmann::json& json)
	{
		m_bs.set_opacity(json.at("opacity"));
		m_bs.set_pos(json.at("x"), json.at("y"));
		m_bs.create({json.at("texture-x"), json.at("texture-y"), json.at("w"), json.at("h")}, json.at("z-level"));
	}
} // namespace galaxy