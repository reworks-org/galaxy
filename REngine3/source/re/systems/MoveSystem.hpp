//
//  MoveSystem.hpp
//  REngine3
//
//  Created by reworks on 07/10/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_MOVESYSTEM_HPP_
#define RENGINE3_MOVESYSTEM_HPP_

#include "re/types/System.hpp"

namespace re
{
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
		void AutoSubmit();

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
		void RemoveEntity(sf::Uint64 e) override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Update the system.
		*/
		void Update();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean();
	private:

	};
}

#endif