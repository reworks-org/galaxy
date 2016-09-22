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

	void RenderSystem::Submit(re::World* world)
	{
		for (auto& it : world->GetEntityDatabase())
		{
			if (it.second->Has<SpriteComponent>())
			{
				m_groups[it.second->Get<SpriteComponent>()->m_group].AddDrawable(it.second->Get<SpriteComponent>());
			}

			if (it.second->Has<TextComponent>())
			{
				m_groups[it.second->Get<TextComponent>()->m_group].AddDrawable(it.second->Get<TextComponent>());
			}

			if (it.second->Has<AnimatedSpriteComponent>())
			{
				m_groups[it.second->Get<AnimatedSpriteComponent>()->m_group].AddAnimated(it.second->Get<AnimatedSpriteComponent>());
			}
		}
	}

	void RenderSystem::Submit(unsigned long group, std::shared_ptr<sf::Drawable> drawable)
	{
		m_groups[group].AddDrawable(drawable);
	}

	void RenderSystem::Submit(unsigned long group, std::shared_ptr<Animated> animated)
	{
		m_groups[group].AddAnimated(animated);
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