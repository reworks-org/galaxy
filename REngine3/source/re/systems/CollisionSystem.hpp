//
//  CollisionSystem.hpp
//  REngine3
//
//  Created by reworks on 8/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_COLLISIONSYSTEM_HPP_
#define RENGINE3_COLLISIONSYSTEM_HPP_

#include <SFML/Graphics/Rect.hpp>

#include "re/types/System.hpp"

namespace re
{
	class CollisionSystem : public System
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Cleans up the systems.
		*/
		~CollisionSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world);

		/*
		* IMPORTS: Vector of collideable tiles.
		* EXPORTS: none
		* PURPOSE: Add tiles to collision system.
		*/
		void SubmitTiles(std::vector<sf::IntRect>& ct);

		/*
		* IMPORTS: sf::IntRect of collision area.
		* EXPORTS: none
		* PURPOSE: To add a space where entitys collide.
		*/
		void AddCollision(sf::IntRect& rect);

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
		void Clean();

	private:
		std::vector<sf::IntRect> m_collisions;
		std::map<std::string, sf::IntRect> m_tilesAroundPlayer;
	};
}

#endif


// import tile collisions amounght others

// check tiles AROUND player...

// implement tile-based movement first.