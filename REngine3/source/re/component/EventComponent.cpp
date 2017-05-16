//
//  EventComponent.cpp
//  REngine3
//
//  Created by reworks on 22/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utility/Log.hpp"
#include "re/debug/imgui/imgui.h"

#include "EventComponent.hpp"

namespace re
{
	EventComponent::EventComponent()
    {
        m_events.clear();
	}

	EventComponent::~EventComponent()
	{
		m_events.clear();
	}

	void EventComponent::init(sol::table& table)
	{
	}

	bool EventComponent::debugFunction(sol::table& table)
	{
		ImGui::Spacing();

		std::string text = "Number of events: " + std::to_string(m_events.size());
		ImGui::Text(text.c_str());
		
		return false;
	}

	void EventComponent::submitOnEvent(EventType type, std::function<void(void)> func)
	{
		auto it = m_events.find(type);
		if (it != m_events.end())
		{
			m_events[type].push_back(func);
		}
		else
		{
			m_events.emplace(type, std::vector<std::function<void(void)>>());
			m_events[type].push_back(func);
		}
	}

	bool EventComponent::isSubscribed(EventType type)
	{
		auto found = m_events.find(type);
		if (found != m_events.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void EventComponent::onEvent(EventType type)
	{
		auto it = m_events.find(type);

        RE_REVERSE_ASSERT_COMPARE(it, m_events.end(), "Tried to access a non-existent event", "EventComponent::onEvent", "EventComponent.cpp", 60);
        
		for (auto& v : it->second)
		{
			v();
		}
	}
}
