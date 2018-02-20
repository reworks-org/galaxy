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
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/TransformComponent.hpp"

#include "CameraTag.hpp"

namespace sl
{
	CameraTag::CameraTag(const sol::table& table)
	{
		m_bounds.m_x = 0;
		m_bounds.m_y = 0;
		m_bounds.m_width = table.get<int>("width");
		m_bounds.m_height = table.get<int>("height");
	}

	CameraTag& CameraTag::operator=(const CameraTag &)
	{
		return *this;
	}

	void CameraTag::update(unsigned int playerEntity)
	{
		Level* level = Locator::m_world->m_currentLevel.get();
		TransformComponent& tc = Locator::m_world->m_registry.get<TransformComponent>(playerEntity);

		m_bounds.m_x = (tc.m_rect.m_x + tc.m_rect.m_width / 2) - m_bounds.m_width / 2;
		m_bounds.m_y = (tc.m_rect.m_y + tc.m_rect.m_height / 2) - m_bounds.m_height / 2;

		if (m_bounds.m_x < 0) { m_bounds.m_x = 0; }
		if (m_bounds.m_y < 0) { m_bounds.m_y = 0; }
		if (m_bounds.m_x > level->getBounds().m_width - m_bounds.m_width) { m_bounds.m_x = level->getBounds().m_width - m_bounds.m_width; }
		if (m_bounds.m_y > level->getBounds().m_height - m_bounds.m_height) { m_bounds.m_y = level->getBounds().m_height - m_bounds.m_height; }
	}
}