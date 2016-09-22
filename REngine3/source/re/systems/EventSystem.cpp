//
//  EventSystem.cpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/component/EventComponent.hpp"

#include "EventSystem.hpp"

namespace re
{
	EventSystem::~EventSystem()
	{
		m_attachedEntitys.clear();
		m_listenerList.clear();
	}

	void EventSystem::Subscribe(std::shared_ptr<Entity> e, EventType type)
	{
		auto it = m_listenerList.find(type);
		if (it != m_listenerList.end())
		{
			m_listenerList[type].push_back(e);
		}
		else
		{
			m_listenerList.emplace(type, std::vector<std::shared_ptr<Entity>>());
			m_listenerList[type].push_back(e);
		}
	}

	void EventSystem::Dispatch(EventType event)
	{
		auto it = m_listenerList.find(event);

		if (it != m_listenerList.end())
		{
			for (auto& v : it->second)
			{
				v->Get<EventComponent>()->OnEvent(event);
			}
		}
	}
}