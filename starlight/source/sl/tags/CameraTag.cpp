///
/// CameraTag.cpp
/// starlight
///
/// Created by reworks on 28/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/graphics/Window.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/TransformComponent.hpp"

#include "CameraTag.hpp"

namespace sl
{
	CameraTag::CameraTag()
		:m_bounds(0.0f, 0.0f, Locator::window->getSize().m_x, Locator::window->getSize().m_y)
	{
	}

	CameraTag::CameraTag(const sol::table& table)
	{
		// Set values from lua table.
		m_bounds.m_x = 0;
		m_bounds.m_y = 0;
		m_bounds.m_width = table.get<int>("width");
		m_bounds.m_height = table.get<int>("height");
	}

	CameraTag::CameraTag(const Rect<float, int>& bounds)
		:m_bounds(bounds)
	{
		// Argument constructor.
	}

	void CameraTag::update(unsigned int playerEntity)
	{
		// Retrieve transformcomponent of player.
		TransformComponent& tc = Locator::world->m_registry.get<TransformComponent>(playerEntity);

		// Calculate new (x, y) of the camera.
		m_bounds.m_x = (tc.m_rect.m_x + tc.m_rect.m_width / 2) - m_bounds.m_width / 2;
		m_bounds.m_y = (tc.m_rect.m_y + tc.m_rect.m_height / 2) - m_bounds.m_height / 2;

		// Then correct values if "out of bounds".
		if (m_bounds.m_x < 0) { m_bounds.m_x = 0; }
		if (m_bounds.m_y < 0) { m_bounds.m_y = 0; }
		if (m_bounds.m_x > Locator::stateMachine->top()->m_bounds.m_width - m_bounds.m_width) { m_bounds.m_x = (float)Locator::stateMachine->top()->m_bounds.m_width - (float)m_bounds.m_width; }
		if (m_bounds.m_y > Locator::stateMachine->top()->m_bounds.m_height - m_bounds.m_height) { m_bounds.m_y = (float)Locator::stateMachine->top()->m_bounds.m_height - (float)m_bounds.m_height; }
	}
}