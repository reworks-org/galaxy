//
//  MoveSystem.hpp
//  Sandbox
//
//  Created by reworks on 21/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef SANDBOX_MOVESYSTEM_HPP_
#define SANDBOX_MOVESYSTEM_HPP_

#include <re/types/System.hpp>

// BASIC MOVE SYSTEM EXAMPLE SYSTEM

namespace re
{
	class World;

	class MoveSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the systems.
		*/
		~MoveSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world) override;

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(Entity* e) override;

		/*
		* IMPORTS: id of entity to remove
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(const std::string& name) override;

		/*
		* IMPORTS: name of entity and the horizontal velocity
		* EXPORTS: none
		* PURPOSE: Move an entity.
		*/
		void Move(const std::string& name, double velocity);

		/*
		* IMPORTS: name of entity and the vertical velocity
		* EXPORTS: none
		* PURPOSE: Make an entity jump.
		*/
		void Jump(const std::string& name, double velocity);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean() override;
	};
}

#endif