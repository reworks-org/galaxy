///
/// World.hpp
/// starlight
///
/// Created by reworks on 09/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_WORLD_HPP_
#define STARLIGHT_WORLD_HPP_

#include <typeindex>
#include <functional>
#include <string_view>
#include <unordered_map>

#include "sl/types/System.hpp"
#include "sl/libs/sol2/sol.hpp"

#ifndef NDEBUG
	#include "sl/libs/loguru/loguru.hpp"
#endif

namespace sl
{
	class World final
	{
	public: 
		///
		/// Construct World.
		///
		World();

		///
		/// Cleans up world.
		///
		~World();

		///
		/// Register an entity.
		///
		/// \param script Script file in the VirtualFS.
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
		/// Update the world.
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
		void registerTag(std::string_view name);

		///
		/// Registers a component with the world.
		///
		/// \param Component - Type of component to register, i.e. AnimationComponent.
		/// \param name - Name of component in string format i.e. "AnimationComponent".
		///
		template<typename Component>
		void registerComponent(std::string_view name);

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

	public:
		sol::state m_lua;
		entt::DefaultRegistry m_registry;

	protected:
		std::unordered_map<std::string_view, std::function<void(entt::DefaultRegistry::entity_type, const sol::table&)>> m_tagAssign;
		std::unordered_map<std::string_view, std::function<void(entt::DefaultRegistry::entity_type, const sol::table&)>> m_componentAssign;
		std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
	};

	template<typename Tag>
	void World::registerTag(std::string_view name)
	{
		#ifdef NDEBUG
			m_tagAssign.emplace(name, [this](entt::DefaultRegistry::entity_type e, const sol::table& table)
			{
				m_registry.assign<Tag>(entt::tag_t{}, e, table);
			});
		#else
			if (m_tagAssign.find(name) != m_tagAssign.end())
			{
				LOG_S(WARNING) << "Attempted to register duplicate tag!";
			}
			else
			{
				m_tagAssign.emplace(name, [this](entt::DefaultRegistry::entity_type e, const sol::table& table)
				{
					if (!m_registry.has<Tag>())
					{
						m_registry.assign<Tag>(entt::tag_t{}, e, table);
					}
					else
					{
						LOG_S(WARNING) << "Attempted to attach duplicate tag!";
					}
				});
			}
		#endif
	}

	template<typename Component>
	void World::registerComponent(std::string_view name)
	{
		#ifdef NDEBUG
			m_componentAssign.emplace(name, [this](entt::DefaultRegistry::entity_type entity, const sol::table& table)
			{
				m_registry.assign<Component>(entity, table);
			});
		#else
			if (m_componentAssign.find(name) != m_componentAssign.end())
			{
				LOG_S(WARNING) << "Attempted to register duplicate component!";
			}
			else
			{
				m_componentAssign.emplace(name, [this](entt::DefaultRegistry::entity_type entity, const sol::table& table)
				{
					m_registry.assign<Component>(entity, table);
				});
			}
		#endif
	}

	template<typename System, typename... Args>
	void World::registerSystem(Args&&... args)
	{
		/// Here, we have two seperate methods. Debug has some more access checks.
		/// We remove the extra checks in release mode to ensure speed.
		#ifdef NDEBUG
			m_systems[std::type_index(typeid(System))] = std::make_unique<System>(std::forward<Args>(args)...);
		#else
			if (m_systems.find(std::type_index(typeid(System))) != m_systems.end())
			{
				LOG_S(WARNING) << "Attempted to register duplicate system!";
			}
			else
			{
				m_systems[std::type_index(typeid(System))] = std::make_unique<System>(std::forward<Args>(args)...);
			}
		#endif
	}

	template<typename System>
	System* World::getSystem()
	{
		/// Here, we have two seperate methods. Debug has some more access checks.
		/// We remove the extra checks in release mode to ensure speed.
		#ifdef NDEBUG
			return dynamic_cast<System*>(m_systems[std::type_index(typeid(System))].get());
		#else
			if (m_systems.find(std::type_index(typeid(System))) != m_systems.end())
			{
				return dynamic_cast<System*>(m_systems[std::type_index(typeid(System))].get());
			}
			else
			{
				LOG_S(WARNING) << "Attempted to access non-existent system.";
				return nullptr;
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
			if (m_systems.find(System::m_id) != m_systems.end())
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