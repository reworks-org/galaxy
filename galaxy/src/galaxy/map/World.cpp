///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/Log.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace map
	{
		World::World(std::string_view file)
		{
			if (load(file))
			{
				parse();
			}
		}

		World::~World()
		{
			m_maps.clear();
		}

		const bool World::load(std::string_view file)
		{
			const auto path = json::parse_from_disk(file);
			if (path != std::nullopt)
			{
				m_json = path.value();
				return true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load tiled world from disk: {0}.", file);
				return false;
			}
		}

		const bool World::parse()
		{
			const auto& map_array = m_json.at("maps");

			for (const auto& obj : map_array)
			{
				auto& map        = m_maps.emplace_back();
				std::string file = obj.at("fileName");
				if (!map.load(file))
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load map: {0}.", file);
					return false;
				}
				else
				{
					if (!map.parse())
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse map: {0}.", file);
						return false;
					}
				}
			}

			return true;
		}
	} // namespace map
} // namespace galaxy