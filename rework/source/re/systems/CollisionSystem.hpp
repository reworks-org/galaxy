//
//  CollisionSystem.hpp
//  rework
//
//  Created by reworks on 28/08/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_COLLISIONSYSTEM_HPP_
#define REWORK_COLLISIONSYSTEM_HPP_

#include "entityx/System.h"
#include "re/math/QuadTree.hpp"

namespace re
{
	class CollisionSystem : public entityx::System<CollisionSystem>
	{
	public:
		///
		/// Constructor.
		///
		CollisionSystem();

		///
		/// Destructor.
		///
		~CollisionSystem() override;
		
		///
		/// Set up the internal quad tree.
		///
		/// \param level Layer of the quadtree (99% of the time this is 0).
		/// \param bounds Bounds of the quadtree (usually camera or level, etc).
		/// \param maxLevels how many levels in the quadtree.
		/// \param maxObjects Maximum number of objects per node.
		///
		void setUpQuadTree(int level, Rect<float, int>& bounds, int maxLevels = 5, int maxObjects = 10);

		///
		/// \brief Update the system.
		///
		/// Dont actually call this, this is called by entity x internal system manager.
		///
		void update(entityx::EntityManager& es, entityx::EventManager& events, entityx::TimeDelta dt) override;
		
		///
		/// Clean up system.
		///
		void clean();

	private:
		QuadTree* m_quadtree;
		std::vector<entityx::Entity> m_possibleCollisions;
	};
}

#endif
