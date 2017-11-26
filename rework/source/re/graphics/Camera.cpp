//
//  Camera.cpp
//  rework
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#include "loguru/loguru.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/components/TransformComponent.hpp"

#include "Camera.hpp"

// https://wiki.allegro.cc/index.php?title=How_to_implement_a_camera
// https://gamedev.stackexchange.com/questions/46228/implementing-a-camera-viewport-to-a-2d-game
// http://lazyfoo.net/tutorials/SDL/30_scrolling/index.php
// https://stackoverflow.com/questions/3614823/2d-platform-game-camera-in-c

namespace re
{
	Camera::Camera(entityx::Entity e)
		:m_entityToFollow(e)
	{
		auto tc = e.component<TransformComponent>();
		auto screenWidth = Locator::get<Window>()->getSize().x;
		auto screenHeight = Locator::get<Window>()->getSize().y;

		if (tc)
		{
			m_bounds.x = (tc->m_rect.x + tc->m_rect.width / 2) - screenWidth / 2;
			m_bounds.y = (tc->m_rect.y + tc->m_rect.height / 2) - screenHeight / 2;
			m_bounds.width = screenWidth;
			m_bounds.height = screenHeight;
		}
		else
		{
			LOG_S(ERROR) << "Attemped to use an entity with no transform component!" << std::endl;
		}
	}

	Camera::Camera(Rect<float, int> bounds)
		:m_bounds(bounds)
	{
	}

	void Camera::update(Level* level)
	{
		auto tc = m_entityToFollow.component<TransformComponent>();

		m_bounds.x = tc->m_rect.x;
		m_bounds.y = tc->m_rect.y;

		if (m_bounds.x < 0) { m_bounds.x = 0; }
		if (m_bounds.y < 0) { m_bounds.y = 0; }
		if (m_bounds.x > LEVEL_WIDTH - m_bounds.width) { m_bounds.x = LEVEL_WIDTH - m_bounds.width; }
		if (m_bounds.y > LEVEL_HEIGHT - m_bounds.height) { m_bounds.y = LEVEL_HEIGHT - m_bounds.height; }
	}
}