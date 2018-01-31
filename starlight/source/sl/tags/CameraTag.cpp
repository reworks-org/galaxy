///
/// CameraTag.cpp
/// starlight
///
/// Created by reworks on 28/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sol2/sol.hpp"

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

	/*
	void Camera::update(Level* level)
	{
		auto tc = m_entityToFollow.component<TransformComponent>();

		m_bounds.x = (tc->m_rect.x + tc->m_rect.width / 2) - m_bounds.width / 2;
		m_bounds.y = (tc->m_rect.y + tc->m_rect.height / 2) - m_bounds.height / 2;

		if (m_bounds.x < 0) { m_bounds.x = 0; }
		if (m_bounds.y < 0) { m_bounds.y = 0; }
		if (m_bounds.x > level->getBounds().width - m_bounds.width) { m_bounds.x = level->getBounds().width - m_bounds.width; }
		if (m_bounds.y > level->getBounds().height - m_bounds.height) { m_bounds.y = level->getBounds().height - m_bounds.height; }

	}
	*/
}