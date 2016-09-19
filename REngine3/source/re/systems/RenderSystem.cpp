//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/entity/World.hpp"
#include "re/graphics/Window.hpp"
#include "re/services/ServiceLocator.hpp"

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
			if (it.second->Has<SpriteComponent>())
			{
				m_groups[it.second->Get<SpriteComponent>()->m_group].AddDrawable(it.second->Get<SpriteComponent>());
			}

			if (it.second->Has<TextComponent>())
			{
				m_groups[it.second->Get<TextComponent>()->m_group].AddDrawable(it.second->Get<TextComponent>());
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
		for (auto& g : m_groups)
		{
			Locator::Get<Window>()->draw(g);
		}
	}
}