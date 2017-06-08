//
//  PhysicsSystem.hpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_PHYSICSSYSTEM_HPP_
#define RENGINE3_PHYSICSSYSTEM_HPP_

#include "re/types/System.hpp"
#include "re/physics/Box2DManager.hpp"

namespace re
{
	class Box2DManager;
	class TMXMap;

	class PhysicsSystem : public System
	{
	public:
		/*
		* IMPORTS: Pointer to manager service, updates per second, velocity iterations and position iterations, usingTilemapCollisions is true if we need to worry about tilemap collisions.
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		PhysicsSystem(Box2DManager* manager, float ups, int32 vi, int32 pi);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the systems.
		*/
		~PhysicsSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void submit(World* world) override;

		/*
		* IMPORTS: Pointer to entity to add.
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
		* PURPOSE: Handles collisions.
		*/
		void update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void clean() override;

	private:
		float m_ups;
		int32 m_velocityIterations;
		int32 m_positionIterations;

		Box2DManager* m_manager;
		std::vector<b2Body*> m_mapCollisions;
	};
}

#endif