///
///  World.hpp
///  rework
///
///  Created by reworks on 09/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_WORLD_HPP_
#define REWORK_WORLD_HPP_

#include <typeindex>
#include <string_view>
#include <unordered_map>

#include "entt/entt.hpp"
#include "sol2/sol_forward.hpp"

namespace re
{
	class World final
	{
		friend class DebugManager;
	public: 
		///
		/// Construct World.
		///
		World() = default;

		///
		/// Cleans up world.
		///
		~World() = default;

		///
		/// Register an entity.
		///
		/// \param script Script file in the VFS.
		///
		void createEntity(std::string_view script);

		///
		/// Automatically create a batch of entitys from a script.
		///
		/// \param batchScript Script file containing all the entity/script (k/v) to register.
		///
		void createEntities(std::string_view batchScript);

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
		/// \param debug - True if the component has a debug() method. False if it does not.
		///
		template<typename T>
		void registerComponent(const std::string& name, bool debug = true);

	private:
		///
		/// Copy Constructor.
		/// Deleted.
		///
		World(const World&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		World(World&&) = delete;

	private:
		std::unordered_map<entt::HashedString::hash_type, std::function<void(const std::uint32_t, const sol::table&)>> m_componentAssign;
		std::unordered_map<entt::HashedString::hash_type, std::function<void(const std::uint32_t)>> m_componentDebug;

	public:
		entt::DefaultRegistry m_registery;
		std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
	};

	template<typename T>
	void World::registerComponent(const std::string& name, bool debug)
	{
		m_componentAssign.emplace(name, [this](const EntitySize e, const sol::table& table)
		{
			m_registery.assign<T>(e, table);
		});

		if (debug)
		{
			m_componentDebug.emplace(name, [this](const EntitySize e)
			{
				m_registery.get<T>(e)->debug();
			});
		}
	}
}

#endif