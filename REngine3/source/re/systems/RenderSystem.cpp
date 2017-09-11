//
//  RenderSystem.cpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/graphics/Window.hpp"
#include "re/graphics/PostEffect.hpp"
#include "re/component/TextComponent.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/ParallaxComponent.hpp"
#include "re/component/TransformComponent.hpp"

#include "RenderSystem.hpp"

namespace re
{
	RenderSystem::RenderSystem(int numOfGroups, Window* window)
	{
		int max = numOfGroups + 1;

		m_groups.clear();
		m_groups.resize(0);
		m_groups.reserve(max);

		for (int i = 0; i < max; i++)
		{
			m_groups.push_back(Group());
		}

		m_outputBuffer.create(window->getSize().x, window->getSize().y);

		m_typeAsString = "RenderSystem";
	}

	RenderSystem::~RenderSystem()
	{
		m_groups.clear();
		m_entitys.clear();
	}

	void RenderSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("RenderSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("RenderSystem", std::type_index(typeid(RenderSystem)));
		}

		if (e->has<SpriteComponent>())
		{
			auto sc = e->get<SpriteComponent>();

			m_groups[sc->m_group].addDrawable(e->m_name, sc->m_sprite, e->get<TransformComponent>());
		}

		if (e->has<TextComponent>())
		{
			auto tc = e->get<TextComponent>();

			m_groups[tc->m_group].addDrawable(e->m_name, tc->m_text, tc->m_text.getTransform());
		}

		if (e->has<ParallaxComponent>())
		{
			auto pc = e->get<ParallaxComponent>();
			auto& pr = pc->getParallaxRects();

			for (auto& it : pr)
			{
				m_groups[it.first].addDrawable(e->m_name, pc->getDrawable(), 
			}
		}
	}

	void RenderSystem::addGenericDrawable(Entity* e, sf::Uint32 group, sf::Drawable* d, sf::Transformable* t)
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

	void RenderSystem::render(Window* window, PostEffect* effect, bool smooth)
	{		
		if (effect != nullptr)
		{
			m_outputBuffer.clear();

			m_outputBuffer.setSmooth(smooth);

			for (auto& g : m_groups)
			{
				m_outputBuffer.draw(g);
			}

			m_outputBuffer.display();

			effect->apply(m_outputBuffer, (*window));
		}
		else
		{
			for (auto& g : m_groups)
			{
				window->draw(g);
			}
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
