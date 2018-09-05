///
/// Serialize.cpp
/// sandbox
///
/// Created by reworks on 20/05/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include <sl/components/EnabledComponent.hpp>
#include <sl/components/AnimationComponent.hpp>
#include <sl/components/TransformComponent.hpp>
#include <sl/components/PhysicsComponent.hpp>
#include <sl/components/RenderComponent.hpp>
#include <sl/tags/CameraTag.hpp>
#include <sl/core/ServiceLocator.hpp>
#include <sl/core/World.hpp>

#include "Serialize.hpp"

Serialize::Serialize(const std::string& path)
	:Serializer(path)
{
}

void Serialize::createGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& source)
{
	std::ofstream saveFile; 
	saveFile.open(m_saveFilePath + saveFileName, std::ofstream::out);
	
	{
		cereal::JSONOutputArchive ar(saveFile);
		createFrameworkSnapshot(ar, source);
		source.snapshot().entities(ar).destroyed(ar).component<sl::AnimationComponent, sl::EnabledComponent, sl::RenderComponent, sl::TransformComponent, sl::PhysicsComponent>(ar).tag<sl::CameraTag>(ar);
	}
	
	saveFile.close();
}

void Serialize::loadGameSnapshot(const std::string& saveFileName, entt::DefaultRegistry& destination)
{
	{
		std::ifstream saveFile;
		saveFile.open(m_saveFilePath + saveFileName, std::ifstream::in);

		{
			cereal::JSONInputArchive ar(saveFile);
			loadFrameworkSnapshot(ar, destination);

			entt::ContinuousLoader<entt::DefaultRegistry::entity_type> loader{ destination };
			loader.entities(ar).destroyed(ar).component<sl::AnimationComponent, sl::EnabledComponent, sl::RenderComponent, sl::TransformComponent, sl::PhysicsComponent>(ar)
				.tag<sl::CameraTag>(ar).orphans().shrink();
		}

		saveFile.close();
	}

	// then we have to update physics entities in a new scope.
	// Iterate over entities, updating their transformcomponent to match the physics component.
	sl::Locator::world->m_registry.view<sl::PhysicsComponent>().each([&](entt::DefaultRegistry::entity_type entity, sl::PhysicsComponent& pc)
	{
		pc.setFixtureEntity(entity);
	});
}
