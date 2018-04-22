///
/// RenderableComponent.cpp
/// starlight
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"

#include "RenderComponent.hpp"

namespace sl
{
	RenderComponent::RenderComponent(entt::Entity entity, const sol::table& table)
	{
		m_opacity = table.get<float>("opacity");
		m_textureName = table.get<std::string>("textureName");
	}

	RenderComponent::RenderComponent(float opacity, const std::string& textureName)
		:m_opacity(opacity), m_textureName(textureName)
	{
	}

	RenderComponent& RenderComponent::operator=(const RenderComponent&)
	{
		return *this;
	}
}