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
		int max = numOfGroups + 1;

		m_groups.clear();
		m_groups.resize(0);
		m_groups.reserve(max);

		for (int i = 0; i < max; i++)
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
			if (it.second->Has<SpriteComponent>() || it.second->Has<TextComponent>() || it.second->Has<AnimatedSpriteComponent>() || it.second->Has<TimeComponent>())
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
			m_groups[e->Get<AnimatedSpriteComponent>()->m_group].AddDrawable(e->m_id, e->Get<AnimatedSpriteComponent>());
		}

		if (e->Has<TimeComponent>())
		{
			m_groups[e->Get<TimeComponent>()->m_group].AddDrawable(e->m_id, e->Get<TimeComponent>());
		}
	}

	void RenderSystem::RemoveEntity(sf::Uint64 e)
	{
		auto it = std::find(m_entitys[e]->m_systemIds.begin(), m_entitys[e]->m_systemIds.end(), typeid(RenderSystem));
		m_entitys[e]->m_systemIds.erase(it);

		for (auto& v : m_groups)
		{
			auto found = v.GetDrawableMap().find(e);
			if (found != v.GetDrawableMap().end())
			{
				v.GetDrawableMap().erase(e);
			}
		}
	}

	void RenderSystem::Render(re::Window* window)
	{
		for (auto& g : m_groups)
		{
			window->draw(g);
		}
	}

	void RenderSystem::Clean()
	{
		for (auto& v : m_groups)
		{
			v.GetDrawableMap().clear();
		}

		m_entitys.clear();
	}
}