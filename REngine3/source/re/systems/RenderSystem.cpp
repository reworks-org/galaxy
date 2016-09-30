//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/graphics/Window.hpp"
#include "re/entity/EntityManager.hpp"
#include "re/services/ServiceLocator.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(int numOfGroups)
	{
		m_groups.clear();
		m_groups.resize(0);
		m_groups.reserve(numOfGroups + 1);

		for (int i = 0; i < numOfGroups; i++)
		{
			m_groups.push_back(Group());
		}
	}

	RenderSystem::~RenderSystem()
	{
		m_groups.clear();
		m_entitys.clear();
	}

	void RenderSystem::AutoSubmit()
	{
		for (auto& it : Locator::Get<EntityManager>()->GetMap())
		{
			if (it.second->Has<SpriteComponent>() || it.second->Has<TextComponent>() || it.second->Has<AnimatedSpriteComponent>())
			{
				AddEntity(it.second);
			}
		}
	}

	void RenderSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.push_back(typeid(RenderSystem));
		
		if (e->Has<SpriteComponent>())
		{
			m_groups[e->Get<SpriteComponent>()->m_group].AddDrawable(e->m_id, e->Get<SpriteComponent>());
		}

		if (e->Has<TextComponent>())
		{
			m_groups[e->Get<TextComponent>()->m_group].AddDrawable(e->m_id, e->Get<TextComponent>());
		}

		if (e->Has<AnimatedSpriteComponent>())
		{
			m_groups[e->Get<AnimatedSpriteComponent>()->m_group].AddAnimated(e->m_id, e->Get<AnimatedSpriteComponent>());
		}
	}

	void RenderSystem::RemoveEntity(sf::Uint64 e)
	{
		auto it = std::find(m_entitys[e]->m_systemIds.begin(), m_entitys[e]->m_systemIds.end(), typeid(RenderSystem));
		m_entitys[e]->m_systemIds.erase(it);

		for (auto& v : m_groups)
		{
			auto foundA = v.GetAnimatedMap().find(e);
			if (foundA != v.GetAnimatedMap().end())
			{
				v.GetAnimatedMap().erase(e);
			}

			auto foundB = v.GetDrawableMap().find(e);
			if (foundB != v.GetDrawableMap().end())
			{
				v.GetDrawableMap().erase(e);
			}
		}
	}

	void RenderSystem::Update(sf::Time dt)
	{
		for (auto& g : m_groups)
		{
			g.Update(dt);
		}
	}

	void RenderSystem::Render(re::Window* window)
	{
		for (auto& g : m_groups)
		{
			window->draw(g);
		}
	}
}