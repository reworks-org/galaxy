///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>
#include <galaxy/fs/FileSystem.hpp>

#include "World.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	World::World() noexcept
		:Manager()
	{
		// TODO: Register defaults.
	}

	World::~World() noexcept
	{
		clear();
	}

	void World::createFromJSON(const std::string& file) noexcept
	{
		// Makes sure the filepath is correct for the current platform.
		auto path = std::filesystem::path(file);
		auto root = nlohmann::json();
		std::ifstream input(path.string(), std::ifstream::in);

		if (input.fail())
		{
			PL_LOG(PL_ERROR, "Failed to open file: " + file);
		}
		else
		{
			// Use JSON stream to deserialize data and parse.
			input >> root;
			auto entity = create();

			// Loop over components
			if (!root.empty())
			{
				for (auto& [key, value] : root.items())
				{
					// Use the assign function to create components for entities without having to know the type.
					m_componentFactory[key](entity, value);
				}
			}
			else
			{
				PL_LOG(PL_WARNING, "Created an entity with no components.");
			}
		}
	}
}