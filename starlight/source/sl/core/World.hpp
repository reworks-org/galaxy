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

#include "sl/libs/sol2/sol.hpp"
#include "sl/types/System.hpp"
#include "sl/libs/loguru/loguru.hpp"

namespace sl
{
	///
	/// The World class. Contains the entitys and systems and other library stuff, like the main lua state.
	///
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
		///
		/// The main lua instance. Use this when doing lua things.
		///
		sol::state m_lua;
		
		///
		/// The registry containing all the game entities.
		///
		entt::DefaultRegistry m_registry;

	private:
		///
		/// Used to allow for tag assigning without having to know the tag type.
		///
		std::unordered_map<std::string_view, std::function<void(entt::DefaultRegistry::entity_type, const sol::table&)>> m_tagAssign;

		///
		/// Used to allow for component assigning without having to know the tag type.
		///
		std::unordered_map<std::string_view, std::function<void(entt::DefaultRegistry::entity_type, const sol::table&)>> m_componentAssign;

		///
		/// Systems storage.
		///
		std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems;
	};

	template<typename Tag>
	void World::registerTag(std::string_view name)
	{
		// Make sure there are no duplicate tags before registering.
		if (m_tagAssign.find(name) != m_tagAssign.end())
		{
			LOG_S(WARNING) << "Attempted to register duplicate tag: " << name;
		}
		else
		{
			m_tagAssign.emplace(name, [this](entt::DefaultRegistry::entity_type entity, const sol::table& table)
			{
				m_registry.assign<Tag>(entt::tag_t{}, entity, table);
			});
		}
	}

	template<typename Component>
	void World::registerComponent(std::string_view name)
	{
		// Make sure there are no duplicate components for the hashmap before registering.
		if (m_componentAssign.find(name) != m_componentAssign.end())
		{
			LOG_S(WARNING) << "Attempted to register duplicate component: " << name;
		}
		else
		{
			m_componentAssign.emplace(name, [this](entt::DefaultRegistry::entity_type entity, const sol::table& table)
			{
				m_registry.assign<Component>(entity, table);
			});
		}
	}

	template<typename System, typename... Args>
	void World::registerSystem(Args&&... args)
	{
		// Make sure no duplicate systems when registering.
		if (m_systems.find(std::type_index(typeid(System))) != m_systems.end())
		{
			LOG_S(WARNING) << "Attempted to register duplicate system!";
		}
		else
		{
			// Create system at system type by forwarding arguments.
			m_systems[std::type_index(typeid(System))] = std::make_unique<System>(std::forward<Args>(args)...);
		}
	}

	template<typename System>
	System* World::getSystem()
	{
		// Return a pointer to the system of the type System.
		return dynamic_cast<System*>(m_systems[std::type_index(typeid(System))].get());
	}

	template<typename System>
	void World::remove()
	{
		// Check to make sure system exists.
		auto type = std::type_index(typeid(System));
		if (m_systems.find(type) != m_systems.end())
		{
			// Then call the systems destructor and erase it from the map.
			m_systems[type].reset();
			m_systems.erase(type);
		}
		else
		{
			LOG_S(WARNING) << "Attempted to remove non-existent system: " << type.name();
		}
	}
}

#endif