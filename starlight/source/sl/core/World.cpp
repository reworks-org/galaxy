///
/// World.cpp
/// starlight
///
/// Created by reworks on 09/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <map>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"

#include "World.hpp"

namespace sl
{
	World::World()
	{
		m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::utf8);

		registerComponent<AnimationComponent>("AnimationComponent");
		registerComponent<ParallaxComponent>("ParallaxComponent");
		registerComponent<ParticleComponent>("ParticleComponent");
		registerComponent<PhysicsComponent>("PhysicsComponent");
		registerComponent<RenderComponent>("RenderComponent");
		registerComponent<TransformComponent>("TransformComponent");

		registerTag<CameraTag>("CameraTag");
	}

	World::~World()
	{
		m_tagAssign.clear();
		m_componentAssign.clear();
		m_systems.clear();
		m_registry.reset();
	}

	void World::createEntity(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::virtualFS->openAsString(script));
		
		entt::DefaultRegistry::entity_type entity = m_registry.create();
		sol::table components = lua.get<sol::table>("entity");

		std::map<std::string_view, sol::table> kvp;
		components.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			kvp.insert({ pair.first.as<const char*>(), pair.second.as<sol::table>() });
		});

		if (components.get<bool>("hasTags"))
		{
			sol::table tags = components.get<sol::table>("tags");
			tags.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				m_tagAssign[pair.first.as<const char*>()](entity, pair.second.as<sol::table>());
			});

			kvp.erase("hasTags");
		}

		for (auto& it : kvp)
		{
			m_componentAssign[it.first](entity, it.second);

			if (it.first == "PhysicsComponent")
			{
				m_registry.get<PhysicsComponent>(entity).setFixtureEntity(entity);
			}
		}
	}

	void World::createEntities(const std::string& batchScript)
	{	
		sol::state lua;
		lua.script(Locator::virtualFS->openAsString(batchScript));

		sol::table world = lua.get<sol::table>("world");
		sol::table entityList = world.get<sol::table>("entitys");
		entityList.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			createEntity(pair.second.as<std::string>());
		});
	}

	void World::event(ALLEGRO_EVENT* event)
	{
		for (auto& systemPair : m_systems)
		{
			systemPair.second->event(event, m_registry);
		}
	}

	void World::update(const double dt)
	{
		for (auto& systemPair : m_systems)
		{
			systemPair.second->update(dt, m_registry);
		}
	}
}