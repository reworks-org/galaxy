///
/// RenderableComponent.cpp
/// starlight
///
///  Created by reworks on 12/01/2018.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

// We have to include this first because of the way imgui auto works with stl types.
#include "RenderComponent.hpp"

#include "sol2/sol.hpp"
#include "imgui/auto/core.h"
#include "imgui/auto/impl_base.h"

namespace sl
{
	RenderComponent::RenderComponent(const sol::table& table)
	{
		table.for_each([&](sol::object value)
		{
			m_renderTypes.push_back(value.as<unsigned int>());
		});
	}

	void RenderComponent::debug()
	{
		ImGui::Auto(m_renderTypes, "RenderTypes");
	}
}