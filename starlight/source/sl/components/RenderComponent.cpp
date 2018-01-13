///
/// RenderableComponent.cpp
/// starlight
///
///  Created by reworks on 12/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"

#include "RenderComponent.hpp"

namespace sl
{
	RenderComponent::RenderComponent(const sol::table& table)
	{
		table.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			m_renderTypes.push_back(pair.second.as<unsigned int>());
		});
	}

	void RenderComponent::debug()
	{
		// to do
	}
}