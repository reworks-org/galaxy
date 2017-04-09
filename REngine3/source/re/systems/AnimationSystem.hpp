//
//  AnimationSystem.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_ANIMATIONSYSTEM_HPP_
#define RENGINE3_ANIMATIONSYSTEM_HPP_

#include "re/types/System.hpp"

namespace sf
{
	class Time;
}

namespace re
{
	class AnimationSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the system.
		*/
		~AnimationSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void submit(World* world) override;

		/*
		* IMPORTS: pointer to entity to add.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void addEntity(Entity* e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void removeEntity(const std::string& name) override;

		/*
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Update the animations in the system.
		*/
		void update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void clean() override;
	};
}

#endif
