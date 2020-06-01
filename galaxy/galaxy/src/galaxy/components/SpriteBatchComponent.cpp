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
	SpriteBatchComponent::SpriteBatchComponent() noexcept
		:BatchedSprite()
	{
	}

	SpriteBatchComponent::SpriteBatchComponent(const nlohmann::json& json) noexcept
	{
		m_opacity = json.at("opacity");
		m_zLevel = json.at("z-level");

		create({ json.at("x") , json.at("y") , json.at("width") , json.at("height") }, m_zLevel);

		move(json.at("x"), json.at("y"));
		rotate(json.at("rotation"));
		scale(json.at("scale"));
	}
}