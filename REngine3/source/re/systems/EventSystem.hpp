//
//  EventSystem.hpp
//  REngine3
//
//  Created by reworks on 23/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_EVENTSYSTEM_HPP_
#define RENGINE3_EVENTSYSTEM_HPP_

#include "re/types/Event.hpp"
#include "re/types/System.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	class World;

	class EventSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the events.
		*/
		~EventSystem() override;
		
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world);

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(std::shared_ptr<Entity> e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(const std::string& name) override;

		/*
		* IMPORTS: Event to dispatch
		* EXPORTS: none
		* PURPOSE: Dispatch an event to subscribed entitys.
		*/
		void Dispatch(EventType type);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean();
	};
}

#endif