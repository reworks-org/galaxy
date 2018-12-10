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
	ParallaxComponent::ParallaxComponent()
		:m_verticalSpeed(0.0f), m_horizontalSpeed(0.0f)
	{
	}

	ParallaxComponent::ParallaxComponent(const sol::table& table)
		:m_verticalSpeed(table.get<float>("verticalSpeed")), m_horizontalSpeed(table.get<float>("horizontalSpeed"))
	{
		// Retrieve the values from the lua table.
	}

	ParallaxComponent::ParallaxComponent(const float verticalSpeed, const float horizontalSpeed)
		:m_verticalSpeed(verticalSpeed), m_horizontalSpeed(horizontalSpeed)
	{
	}
}