///
/// TransformComponent.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "TransformComponent.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	TransformComponent::TransformComponent() noexcept
		:m_transform()
	{
		m_transform.move(0.0f, 0.0f);
	}

	TransformComponent::TransformComponent(const nlohmann::json& json) noexcept
	{
		m_transform.move(json.at("x"), json.at("y"));
		m_transform.rotate(json.at("rotation"));
		m_transform.scale(json.at("scale"));
	}
}