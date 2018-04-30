///
/// ParallaxComponent.cpp
/// starlight
///
/// Created by reworks on 04/02/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/libs/sol2/sol.hpp"

#include "ParallaxComponent.hpp"

namespace sl
{
	ParallaxComponent::ParallaxComponent(const sol::table& table)
	{
		m_verticalSpeed = table.get<float>("verticalSpeed");
		m_horizontalSpeed = table.get<float>("horizontalSpeed");
	}

	ParallaxComponent& ParallaxComponent::operator=(const ParallaxComponent&)
	{
		return *this;
	}
}