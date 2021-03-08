///
/// TiledWorld.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include "galaxy/error/Log.hpp"
#include "galaxy/scripting/JSONUtils.hpp"

#include "TiledWorld.hpp"

namespace galaxy
{
	namespace map
	{
		TiledWorld::TiledWorld(std::string_view file)
		{
			if (load(file))
			{
				parse();
			}
		}

		TiledWorld::~TiledWorld()
		{
			m_maps.clear();
		}

		const bool TiledWorld::load(std::string_view file)
		{
			const auto path = json::parse_from_disk(file);
			if (path != std::nullopt)
			{
				m_json = path.value();
				return true;
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load tiled TiledWorld from disk: {0}.", file);
				return false;
			}
		}

		const bool TiledWorld::parse()
		{
			const auto& map_array = m_json.at("maps");

			for (const auto& obj : map_array)
			{
				std::string file = obj.at("fileName");
				const auto id    = std::filesystem::path(file).stem().string();
				Map map;

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
				
				m_maps.emplace(id, std::move(map));
			}

			return true;
		}

		map::Map* TiledWorld::get_map(std::string_view map) noexcept
		{
			const auto str = static_cast<std::string>(map);
			if (m_maps.contains(str))
			{
				return &m_maps[str];
			}
			else
			{
				return nullptr;
			}
		}

		const MapHolder& TiledWorld::get_maps() const noexcept
		{
			return m_maps;
		}

		const nlohmann::json& TiledWorld::get_raw() const noexcept
		{
			return m_json;
		}
	} // namespace map
} // namespace galaxy