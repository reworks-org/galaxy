//
//  World.hpp
//  rework
//
//  Created by reworks on 9/07/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_WORLD_HPP_
#define REWORK_WORLD_HPP_

#include "sol2/sol.hpp"
#include "entityx/entityx.h"
#include "re/types/Service.hpp"

namespace re
{
	class World : public Service
	{
		friend class DebugManager;
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
		/// \brief Update the world.
		///
		/// \param dt timePerFrame from application loop, or delta time.
		/// 
		void update(double dt);

		///
		/// Registers a component with the world.
		///
		/// \param T - Type of component to register, i.e. AnimationComponent.
		/// \param name - Name of component in string format i.e. "AnimationComponent".
		///
		template<typename T>
		void registerComponent(const std::string& name);

	public:
		entityx::EventManager m_eventManager;
		entityx::EntityManager m_entityManager;
		entityx::SystemManager m_systemManager;

	private:
		std::unordered_map<std::string, entityx::Entity> m_entitys;
		std::unordered_map<std::string, std::string> m_entityScripts;
		std::unordered_map<std::string, std::function<void(entityx::Entity&, sol::table&)>> m_componentAssign;
		std::unordered_map<std::string, std::function<void(entityx::Entity&)>> m_componentDebug;
	};

	template<typename T>
	void World::registerComponent(const std::string& name)
	{
		// push back a lambda that calls e->assign and one that calls e->debug.
		m_componentAssign.emplace(name, [](entityx::Entity& e, sol::table& table)
		{
			e.assign<T>(e, table);
		});

		m_componentDebug.emplace(name, [](entityx::Entity& e)
		{
			e.component<T>()->debug();
		});
	}
}

#endif