///
/// RenderableComponent.cpp
/// starlight
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"
#include "sl/libs/loguru/loguru.hpp"

#include "RenderComponent.hpp"

namespace sl
{
	RenderComponent::RenderComponent()
	{
		// Throw an exception if this class is default constructed.
		LOG_S(FATAL) << "Tried to construct a default-initialized RenderComponent!";
	}

	RenderComponent::RenderComponent(const sol::table& table)
	{
		// Retrieve the values from the lua table.
		m_opacity = table.get<float>("opacity");
		m_textureName = table.get<std::string>("textureName");
	}

	RenderComponent::RenderComponent(float opacity, const std::string& textureName)
		:m_opacity(opacity), m_textureName(textureName)
	{
		// Constructs the component from parameters instead of lua.
	}

	RenderComponent& RenderComponent::operator=(const RenderComponent&)
	{
		return *this;
	}
}