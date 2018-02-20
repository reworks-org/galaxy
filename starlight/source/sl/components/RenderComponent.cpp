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
		table.for_each([&](sol::object key, sol::object value)
		{
			m_renderTypes.push_back(value.as<unsigned int>());
		});
	}

	RenderComponent& RenderComponent::operator=(const RenderComponent&)
	{
		return *this;
	}
}