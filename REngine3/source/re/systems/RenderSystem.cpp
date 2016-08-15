//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/services/World.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/SpriteComponent.hpp"
#include "re/component/PositionComponent.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(int numOfGroups)
	{
		for (int i = 0; i < numOfGroups; i++)
		{
			m_groups.push_back(std::unique_ptr<Group>(new Group()));
		}
	}

	RenderSystem::~RenderSystem()
	{
		m_groups.clear();
		m_attachedEntitys.clear();
	}

	void RenderSystem::Submit()
	{
		for (auto& it : Locator::Get<World>()->GetEntityDatabase())
		{
			if (it.second->Has<SpriteComponent>())
			{
				m_groups[it.second->Get<SpriteComponent>()->m_group]->AddEntity(it.second);
			}
		}
	}

	void RenderSystem::Update()
	{
		for (auto& it : Locator::Get<World>()->GetEntityDatabase())
		{
			it.second->Get<SpriteComponent>()->Update({ it.second->Get<PositionComponent>()->x, it.second->Get<PositionComponent>()->y });
		}
	}

	void RenderSystem::Render()
	{
		for (auto& g : m_groups)
		{
			Locator::Get<World>()->m_window.draw(g);
		}
	}
}