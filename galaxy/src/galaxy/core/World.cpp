///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/components/All.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "World.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	World::World()
	    : Manager {}, m_phys_world {}
	{
		register_component<galaxy::AnimationComponent>("AnimationComponent");
		register_component<galaxy::EnabledComponent>("EnabledComponent");
		register_component<galaxy::MusicComponent>("MusicComponent");
		register_component<galaxy::PhysicsComponent>("PhysicsComponent");
		register_component<galaxy::ShaderComponent>("ShaderComponent");
		register_component<galaxy::SoundComponent>("SoundComponent");
		register_component<galaxy::SpriteBatchComponent>("SpriteBatchComponent");
		register_component<galaxy::SpriteComponent>("SpriteComponent");
		
	}

	World::World(const glm::vec2& gravity)
	    : Manager {}, m_phys_world {gravity}
	{
		register_component<galaxy::AnimationComponent>("AnimationComponent");
		register_component<galaxy::EnabledComponent>("EnabledComponent");
		register_component<galaxy::MusicComponent>("MusicComponent");
		register_component<galaxy::PhysicsComponent>("PhysicsComponent");
		register_component<galaxy::ShaderComponent>("ShaderComponent");
		register_component<galaxy::SoundComponent>("SoundComponent");
		register_component<galaxy::SpriteBatchComponent>("SpriteBatchComponent");
		register_component<galaxy::SpriteComponent>("SpriteComponent");
	}

	World::~World()
	{
		clear();
	}

	const sr::Entity World::create_from_json(std::string_view file)
	{
		// Makes sure the filepath is correct for the current platform.
		sr::Entity entity   = 0;
		entity              = create();
		nlohmann::json root = galaxy::json::parse_from_disk(file);

		// Loop over components
		if (!root.empty())
		{
			for (auto& [key, value] : root.items())
			{
				// Use the assign function to create components for entities without having to know the type.
				m_component_factory[key](entity, value);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_WARNING, "Created an entity with no components.");
		}

		return entity;
	}

	rs::World& World::phys_world()
	{
		return m_phys_world;
	}
} // namespace galaxy