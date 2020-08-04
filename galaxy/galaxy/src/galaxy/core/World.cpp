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
	World::World()
	    : Manager {}
	{
	}

	World::~World()
	{
		clear();
	}

	const sr::Entity World::create_from_json(std::string_view file)
	{
		// Makes sure the filepath is correct for the current platform.
		sr::Entity entity = 0;
		auto path         = std::filesystem::path {file};
		std::ifstream ifs;
		ifs.open(path.string(), std::ifstream::in);

		if (!ifs.good())
		{
			PL_LOG(PL_ERROR, "Failed to open file: {0}.", path.string());
		}
		else
		{
			nlohmann::json root;

			// Use JSON stream to deserialize data and parse.
			ifs >> root;
			entity = create();

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
				PL_LOG(PL_WARNING, "Created an entity with no components.");
			}
		}

		return entity;
	}
} // namespace galaxy