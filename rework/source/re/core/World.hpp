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
		/// \brief Register a function that assigns user components to entitys.
		///
		/// See m_assignUserComponents documentation.
		///
		/// \param aucf Function pointer. return type: bool. params: std::string, sol::table, ex::entity.
		///
		void registerAssignUserComponentsFunction(std::function<bool(const std::string&, sol::table&, ex::Entity&)>& aucf);

	private:
		///
		/// System implementation of assignUserComponents()
		///
		bool assignSystemComponents(const std::string& name, sol::table& table, ex::Entity& e);

		///
		/// \brief Assigns user components to entitys.
		///
		/// See assignSystemComponents() implementation on how to implement.
		///
		/// \param name Name of Component to assign.
		/// \param table sol::table of component data.
		/// \param e Entity to assign components to.
		///
		/// \return bool True if it managed to assign an entity, false if it could not assign a component to the entity.
		///
		std::function<bool(const std::string&, sol::table&, ex::Entity&)> m_assignUserComponents;

	public:
		ex::EventManager m_eventManager;
		ex::EntityManager m_entityManager;
		ex::SystemManager m_systemManager;

	private:
		std::unordered_map<std::string, ex::Entity> m_entitys;
	};
}

#endif