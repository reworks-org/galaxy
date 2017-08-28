//
//  CollisionSystem.hpp
//  REngine3
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_COLLISIONSYSTEM_HPP_
#define RENGINE3_COLLISIONSYSTEM_HPP_

#include "re/types/System.hpp"
#include "re/physics/QuadTree.hpp"

namespace re
{
	class CollisionSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Constructor.
		*/
		CollisionSystem();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the system.
		*/
		~CollisionSystem() override;

		/*
		* IMPORTS: id of entity to add and its component list.
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
		* IMPORTS: See QuadTree Constructor.
		* EXPORTS: none
		* PURPOSE: Set up quadtree. See QuadTree Constructor.
		*/
		void setUpQuadTree(int level, sf::Rect<int>& bounds, int maxLevels = 5, int maxObjects = 10);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Handles collisions.
		*/
		void update();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void clean() override;

	private:
		QuadTree* m_quadtree;
	};
}

#endif
