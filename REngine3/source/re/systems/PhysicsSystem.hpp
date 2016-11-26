//
//  PhysicsSystem.hpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_PHYSICSSYSTEM_HPP_
#define RENGINE3_PHYSICSSYSTEM_HPP_

#include <SFML/Graphics/Rect.hpp>

#include "Box2DManager.hpp"
#include "re/types/System.hpp"

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
		PhysicsSystem(Box2DManager* manager, double ups, double vi, double pi);

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
		void AutoSubmit(World* world) override;

		/*
		* IMPORTS: Vector of collideable tiles.
		* EXPORTS: none
		* PURPOSE: Add tiles to collision system.
		*/
		void SubmitTiles(TMXMap* map);

		/*
		* IMPORTS: Pointer to entity to add.
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
		* IMPORTS: delta time
		* EXPORTS: none
		* PURPOSE: Handles collisions.
		*/
		void Update(sf::Time dt);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean() override;

	private:
		double m_ups;
		int m_velocityIterations;
		int m_positionIterations;

		Box2DManager* m_manager;
		std::vector<b2Body*> m_mapCollisions;
	};
}

#endif