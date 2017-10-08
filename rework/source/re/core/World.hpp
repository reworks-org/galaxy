//
//  World.hpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_WORLD_HPP_
#define REWORK_WORLD_HPP_

#include <typeindex>

#include "entityx/entityx.h"
#include "re/types/Service.hpp"

namespace re
{
	class World : public Service
	{
	public: 
		///
		/// \brief Construct World.
		///
		World();

		///
		/// Cleans up world.
		///
		~World() override;

		///
		/// Register an entity.
		///
		/// \param script Script file in the VFS.
		///
		void createEntity(const std::string& script);

		///
		/// Automatically create a batch of entitys from a script.
		///
		/// \param batchScript Script file containing all the entity/script (k/v) to register.
		///
		void createEntities(const std::string& batchScript);

		///
		/// \brief Kill an entity.
		///
		/// Removes an entity from m_alive and moves it to m_dead.
		///
		/// \param name Name of entity to kill.
		///
		//void killEntity(const std::string& name);

		///
		/// \brief Revive an entity.
		///
		/// Removes an entity from m_dead and moves it to m_alive.
		///
		/// \param name Name of entity to revive.
		///
		//void reviveEntity(const std::string& name);

		///
		/// \brief Update the world.
		///
		/// \param dt timePerFrame from application loop, or delta time.
		/// 
		void update(double dt);

	public:
		ex::EventManager m_eventManager;
		ex::EntityManager m_entityManager;
		ex::SystemManager m_systemManager;
	};
}

#endif