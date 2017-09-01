//
//  EventSystem.cpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include <SFML/System/Time.hpp>

#include "re/app/World.hpp"
#include "re/component/EventComponent.hpp"

#include "EventSystem.hpp"

namespace re
{
	EventSystem::EventSystem()
	:m_typeAsString("EventSystem")
	{
	}

	EventSystem::~EventSystem()
	{
		m_entitys.clear();
	}

	void EventSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("EventSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("EventSystem", std::type_index(typeid(EventSystem)));
		}

		m_entitys.emplace(e->m_name, e);
	}

	void EventSystem::removeEntity(const std::string& name)
	{
		m_entitys.erase(name);
	}

	void EventSystem::dispatch(EventType type)
	{
		for (auto& it : m_entitys)
		{
			EventComponent* e = it.second->get<EventComponent>();
			if (e->isSubscribed(type))
			{
				e->onEvent(type);
			}
		}
	}

	void EventSystem::clean()
	{
		m_entitys.clear();
	}
}
