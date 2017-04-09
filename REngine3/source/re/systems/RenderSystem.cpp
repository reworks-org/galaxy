//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/graphics/Window.hpp"
#include "re/component/TextComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/TransformComponent.hpp"

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

	void RenderSystem::submit(World* world)
	{
		for (auto& it : world->getAliveEntitys())
		{
			if (it.second.has<SpriteComponent>() || it.second.has<TextComponent>())
			{
				addEntity(&it.second);
			}
		}
	}

	void RenderSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("RenderSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("RenderSystem", std::type_index(typeid(RenderSystem)));
		}

		if (e->has<SpriteComponent>())
		{
			m_groups[e->get<SpriteComponent>()->m_group].addDrawable(e->m_name, e->get<SpriteComponent>(), e->get<TransformComponent>());
		}

		if (e->has<TextComponent>())
		{
			m_groups[e->get<TextComponent>()->m_group].addDrawable(e->m_name, e->get<TextComponent>(), e->get<TransformComponent>());
		}
	}

	void RenderSystem::addGenericDrawable(Entity* e, sf::Uint32 group, std::shared_ptr<sf::Drawable> d, std::shared_ptr<sf::Transformable> t)
	{
		if (e->m_systemIds.find("RenderSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("RenderSystem", std::type_index(typeid(RenderSystem)));
		}

		m_groups[group].addDrawable(e->m_name, d, t);
	}

	void RenderSystem::removeEntity(const std::string& name)
	{
		for (auto& v : m_groups)
		{
			auto found = v.getDrawableMap().find(name);
			if (found != v.getDrawableMap().end())
			{
				v.getDrawableMap().erase(name);
			}
		}
	}

	void RenderSystem::render(Window* window)
	{
		for (auto& g : m_groups)
		{
			window->draw(g);
		}
	}

	void RenderSystem::clean()
	{
		for (auto& v : m_groups)
		{
			v.getDrawableMap().clear();
		}

		m_entitys.clear();
	}
}
