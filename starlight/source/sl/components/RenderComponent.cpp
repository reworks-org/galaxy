///
/// RenderableComponent.cpp
/// starlight
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/graphics/TextureAtlas.hpp"

#include "RenderComponent.hpp"

namespace sl
{
	RenderComponent::RenderComponent()
		:m_opacity(1.0f), m_textureName(Locator::textureAtlas->m_nullTexture)
	{
	}

	RenderComponent::RenderComponent(const sol::table& table)
	{
		// Retrieve the values from the lua table.
		m_opacity = table.get<float>("opacity");
		m_textureName = table.get<std::string>("textureName");
	}

	RenderComponent::RenderComponent(float opacity, const std::string& atlasID)
		:m_opacity(opacity), m_textureName(atlasID)
	{
		// Constructs the component from parameters instead of lua.
	}

	RenderComponent& RenderComponent::operator=(const RenderComponent&)
	{
		return *this;
	}
}