///
/// World.cpp
/// starlight
///
/// Created by reworks on 09/07/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/fs/VirtualFS.hpp"
#include "sl/tags/CameraTag.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/components/RenderComponent.hpp"
#include "sl/components/EnabledComponent.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/components/ParticleComponent.hpp"
#include "sl/components/ParallaxComponent.hpp"
#include "sl/components/AnimationComponent.hpp"
#include "sl/components/TransformComponent.hpp"
#include "sl/components/ScrollingBackgroundComponent.hpp"

#include "World.hpp"

namespace sl
{
	World::World()
	:m_scriptFolderPath(""), m_textureFolderPath(""), m_musicFolderPath(""), m_soundFolderPath("")
	{
		// Register the library components and tags.
		registerComponent<AnimationComponent>("AnimationComponent");
		registerComponent<EnabledComponent>("EnabledComponent");
		registerComponent<ParallaxComponent>("ParallaxComponent");
		registerComponent<ParticleComponent>("ParticleComponent");
		registerComponent<PhysicsComponent>("PhysicsComponent");
		registerComponent<RenderComponent>("RenderComponent");
		registerComponent<ScrollingBackgroundComponent>("ScrollingBackgroundComponent");
		registerComponent<TransformComponent>("TransformComponent");

		registerTag<CameraTag>("CameraTag");
	}

	World::~World()
	{
		// Clean up data.
		m_tagAssign.clear();
		m_componentAssign.clear();
		m_systems.clear();
		m_registry.reset();
	}

	entt::DefaultRegistry::entity_type World::createEntity(const std::string& script)
	{
		std::string fullPath = m_scriptFolderPath + script;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));
		
		// Create entity.
		entt::DefaultRegistry::entity_type entity = m_registry.create();
		sol::table components = Locator::lua->get<sol::table>("entity");

		// Loop over components
		if (!components.empty())
		{
			components.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				// Use the assign function to create components for entities without having to know the type.
				m_componentAssign[entt::HashedString(pair.first.as<const char*>())](entity, pair.second.as<sol::table>());

				// Then if its the physics component, we set up the fixture collision callback entity.
				if (pair.first.as<const char*>() == "PhysicsComponent")
				{
					m_registry.get<PhysicsComponent>(entity).setFixtureEntity(entity);
				}
			});
		}

		// Loop over tags and assign.
		auto tags = components["Tags"];
		if (tags.valid())
		{
			sol::table tags = components.get<sol::table>("tags");
			tags.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				// Use the assign function to create tags for entities without having to know the type.
				m_tagAssign[entt::HashedString(pair.first.as<const char*>())](entity, pair.second.as<sol::table>());
			});
		}

		return entity;
	}

	void World::createEntities(const std::string& batchScript)
	{	
		std::string fullPath = m_scriptFolderPath + batchScript;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));

		sol::table entityList = Locator::lua->get<sol::table>("entityList");
		entityList.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			// For each entity in list, create that entity.
			createEntity(pair.second.as<std::string>());
		});
	}

	void World::createDuplicateEntities(const std::string& script)
	{
		std::string fullPath = m_scriptFolderPath + script;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));

		sol::table table = Locator::lua->get<sol::table>("entity");
		unsigned int count = table.get<unsigned int>("count");

		for (unsigned int i = 0; i < count; ++i)
		{
			// Create entity.
			entt::DefaultRegistry::entity_type entity = m_registry.create();

			// Loop over components
			if (!table.empty())
			{
				table.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					// Use the assign function to create components for entities without having to know the type.
					m_componentAssign[entt::HashedString(pair.first.as<const char*>())](entity, pair.second.as<sol::table>());

					// Then if its the physics component, we set up the fixture collision callback entity.
					if (pair.first.as<const char*>() == "PhysicsComponent")
					{
						m_registry.get<PhysicsComponent>(entity).setFixtureEntity(entity);
					}
				});
			}

			// Loop over tags and assign.
			sol::table tags = table.get<sol::table>("tags");
			if (!tags.empty())
			{
				tags.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					// Use the assign function to create tags for entities without having to know the type.
					m_tagAssign[entt::HashedString(pair.first.as<const char*>())](entity, pair.second.as<sol::table>());
				});
			}
		}
	}

	void World::update(const double dt)
	{
		// Update systems.
		for (auto& pair : m_systems)
		{
			pair.second->update(dt, m_registry);
		}
	}
}