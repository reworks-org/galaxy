///
///  World.hpp
///  starlight
///
///  Created by reworks on 09/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_WORLD_HPP_
#define STARLIGHT_WORLD_HPP_

#include <string_view>
#include <unordered_map>

#include "sl/types/System.hpp"
#include "sol2/sol_forward.hpp"
#include "sl/mapping/Level.hpp"
#include "sl/types/ServiceLocator.hpp"
#include "entt/core/hashed_string.hpp"

namespace sl
{
	class World final : public ServiceLocator<World>
	{
	public: 
		///
		/// Construct World.
		///
		World() = default;

		///
		/// Cleans up world.
		///
		~World() override = default;

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
		/// Process world events.
		///
		/// \param event Event data structure from application class.
		/// 
		void event(ALLEGRO_EVENT* event);

		///
		/// \brief Update the world.
		///
		/// \param dt timePerFrame from application loop, or delta time.
		/// 
		void update(const double dt);

		///
		/// Registers a tag with the world.
		///
		/// \param Tag - Type of tag to register, i.e. PlayerTag.
		/// \param name - Name of tag in string format i.e. "PlayerTag".
		///
		template<typename Tag>
		void registerTag(entt::HashedString name);

		///
		/// Registers a component with the world.
		///
		/// \param Component - Type of component to register, i.e. AnimationComponent.
		/// \param name - Name of component in string format i.e. "AnimationComponent".
		/// \param debug - True if the component has a debug() method. False if it does not.
		///
		template<typename Component>
		void registerComponent(entt::HashedString name, bool debug = true);

		///
		/// Registers a system with the world.
		///
		/// \param System Type of system to register.
		/// \param Args Argument(s) for system contructor.
		///
		template<typename System, typename... Args>
		void registerSystem(Args&&... args);

		///
		/// Retrieve a system.
		///
		/// \param System Template type. Type of system to fetch.
		///
		/// \return Returns a pointer to the system object.
		///
		template<typename System>
		System* getSystem();

		///
		/// \brief Remove a system.
		///
		/// This de-registers the system, so you will need to call World::registerSystem.
		///
		/// \param System Template type to remove.
		///
		template<typename System>
		void remove();

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
		std::uint32_t m_systemIDCounter = 0;
		std::unordered_map<entt::HashedString::hash_type, std::function<void(entt::Entity)>> m_tagAssign;
		std::unordered_map<entt::HashedString::hash_type, std::function<void(entt::Entity, const sol::table&)>> m_componentAssign;
		std::unordered_map<entt::HashedString::hash_type, std::function<void(entt::Entity)>> m_componentDebug;

	public:
		entt::DefaultRegistry m_registery;
		std::unique_ptr<Level> m_currentLevel;
		std::unordered_map<std::uint32_t, std::unique_ptr<System>> m_systems;
	};

	template<typename Tag>
	void World::registerTag(entt::HashedString name)
	{
		#ifdef NDEBUG
			m_tagAssign.emplace(name, [this](entt::Entity e)
			{
				m_registery.attach<Tag>(e);
			});
		#else
			if (m_tagAssign.find(name) != m_tagAssign.end())
			{
				m_tagAssign.emplace(name, [this](entt::Entity e)
				{
					if (!m_registery.has<Tag>())
					{
						m_registery.attach<Tag>(e);
					}
					else
					{
						LOG_S(WARNING) << "Attempted to attach duplicate tag!";
					}
				});
			}
			else
			{
				LOG_S(WARNING) << "Attempted to register duplicate tag!";
			}
		#endif
	}

	template<typename Component>
	void World::registerComponent(entt::HashedString name, bool debug)
	{
		#ifdef NDEBUG
			m_componentAssign.emplace(name, [this](entt::Entity e, const sol::table& table)
			{
				m_registery.assign<Component>(e, table);
			});

			if (debug)
			{
				m_componentDebug.emplace(name, [this](entt::Entity e)
				{
					m_registery.get<Component>(e)->debug();
				});
			}
		#else
			if (m_componentAssign.find(name) != m_componentAssign.end())
			{
				m_componentAssign.emplace(name, [this](entt::Entity e, const sol::table& table)
				{
					m_registery.assign<Component>(e, table);
				});
			}
			else
			{
				LOG_S(WARNING) << "Attempted to register duplicate component!";
			}

			if (debug)
			{
				if (m_componentDebug.find(name) != m_componentDebug.end())
				{
					m_componentDebug.emplace(name, [this](entt::Entity e)
					{
						m_registery.get<Component>(e)->debug();
					});
				}
				else
				{
					LOG_S(WARNING) << "Attempted to register duplicate component debug function!";
				}
			}
		#endif
	}

	template<typename System, typename... Args>
	void World::registerSystem(Args&&... args)
	{
		/// Here, we have two seperate methods. Debug has some more access checks.
		/// We remove the extra checks in release mode to ensure speed.
		#ifdef NDEBUG
			System::m_id = m_systemIDCounter;
			m_systems[System::m_id] = std::make_unique<System>(std::forward<Args>(args) ...);
			++m_systemIDCounter;
		#else
			if (m_systems.find(System::m_id) != m_systems.cend())
			{
				System::m_id = m_systemIDCounter;
				m_systems[System::m_id] = std::make_unique<System>(std::forward<Args>(args)...);
				++m_systemIDCounter;
			}
			else
			{
				LOG_S(WARNING) << "Attempted to register duplicate system!";
			}
		#endif
	}

	template<typename System>
	System* World::getSystem()
	{
		/// Here, we have two seperate methods. Debug has some more access checks.
		/// We remove the extra checks in release mode to ensure speed.
		#ifdef NDEBUG
			return dynamic_cast<System*>(m_systems[System::m_id].get());
		#else
			if (m_systems.find(System::m_id) != m_systems.cend())
			{
				return dynamic_cast<System*>(m_systems[System::m_id].get());
			}
			else
			{
				LOG_S(ERROR) << "Attempted to access non-existent system.";
			}
		#endif
	}

	template<typename System>
	void World::remove()
	{
		/// Here, we have two seperate methods. Debug has some more access checks.
		/// We remove the extra checks in release mode to ensure speed.
		#ifdef NDEBUG
			m_systems[System::m_id].reset();
			m_systems.erase(System::m_id);
		#else
			if (m_systems.find(System::m_id) != m_systems.cend())
			{
				m_systems[System::m_id].reset();
				m_systems.erase(System::m_id);
			}
			else
			{
				LOG_S(WARNING) << "Attempted to remove non-existent system.";
			}
		#endif
	}
}

#endif