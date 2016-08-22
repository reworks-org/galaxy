//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/entity/World.hpp"
#include "re/services/ServiceLocator.hpp"

#include "re/component/TextComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/PositionComponent.hpp"
#include "re/component/AnimatedSpriteComponent.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(int numOfGroups)
	{
		for (int i = 0; i < numOfGroups; i++)
		{
			m_groups.push_back(Group());
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
			if (it.second->Has<SpriteComponent>() || it.second->Has<TextComponent>() || it.second->Has<AnimatedSpriteComponent>())
			{
				m_groups[it.second->Get<PositionComponent>()->m_group].AddEntity(it.second);
			}
		}
	} 

	void RenderSystem::Update()
	{
		for (auto& g : m_groups)
		{
			g.Update();
		}
	}

	void RenderSystem::Render()
	{
		Locator::Get<World>()->m_window.clear(sf::Color::White);

		for (auto& g : m_groups)
		{
			Locator::Get<World>()->m_window.draw(g);
		}

		Locator::Get<World>()->m_window.display();
	}
}