//
//  AnimationSystem.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//  Code modified from: https://github.com/miguelmartin75/anax/blob/master/examples/common/include/Systems/AnimationSystem.hpp
//  See original file for details.

#include "re/app/World.hpp"
#include "re/component/SpriteComponent.hpp"
#include "re/component/AnimationComponent.hpp"

#include "AnimationSystem.hpp"

namespace re
{
	AnimationSystem::~AnimationSystem()
	{
		m_entitys.clear();
	}

	void AnimationSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second.Has<AnimationComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void AnimationSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.emplace("AnimationSystem", std::type_index(typeid(AnimationSystem)));
		m_entitys.emplace(e->m_name, e);
	}

	void AnimationSystem::RemoveEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void AnimationSystem::Update(sf::Time dt)
	{
		for (auto& e : m_entitys)
		{
			auto animation = e.second->Get<AnimationComponent>();
			auto sprite = e.second->Get<SpriteComponent>();

		}
	}

	void AnimationSystem::SetFPS(unsigned int fps)
	{
		m_fps = fps;
	}

	void AnimationSystem::Clean()
	{
		m_entitys.clear();
	}
}