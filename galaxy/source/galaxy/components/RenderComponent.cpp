///
/// RenderableComponent.cpp
/// galaxy
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"

#include "RenderComponent.hpp"

namespace galaxy
{
	RenderComponent::RenderComponent()
		:m_opacity(1.0f), m_textureName(Locator::textureAtlas->m_nullTexture)
	{
	}

	RenderComponent::RenderComponent(const sol::table& table)
	:m_textureName(table.get<std::string>("textureName")), m_opacity(table.get<float>("opacity"))
	{
	}

	RenderComponent::RenderComponent(float opacity, const std::string& atlasID)
		:m_opacity(opacity), m_textureName(atlasID)
	{
	}
}