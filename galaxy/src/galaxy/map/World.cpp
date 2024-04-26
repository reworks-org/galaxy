///
/// World.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "World.hpp"

namespace galaxy
{
	namespace map
	{
		World::World()
			: m_current {nullptr}
			, m_loaded {false}
		{
		}

		World::World(const std::string& file)
			: m_current {nullptr}
			, m_loaded {false}
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load map from constructor. File is '{0}'.", file);
			}
		}

		World::~World()
		{
		}

		bool World::load(const std::string& file)
		{
			auto  result = false;
			auto& fs     = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto  data   = fs.read_binary(file);
			if (!data.empty())
			{
				try
				{
					m_file = file;
					m_project.loadFromMemory(data.data(), data.size());
					result = m_loaded = true;
				}
				catch (const std::exception& e)
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to load map '{0}' because {1}.", file, e.what());
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to read map '{0}'.", file);
			}

			return result;
		}

		void World::parse()
		{
			if (m_project.allWorlds().size() > 1)
			{
				GALAXY_LOG(GALAXY_WARNING, "Galaxy does not support LDTK multi-worlds.");
			}

			for (auto& level : m_project.getWorld().allLevels())
			{
				auto& map = m_maps[level.name];
				map.load(level);
			}
		}

		void World::clear()
		{
			m_loaded  = false;
			m_file    = "";
			m_current = nullptr;
			m_maps.clear();
		}

		void World::set_active(const std::string& map)
		{
			if (m_maps.contains(map))
			{
				m_current = &m_maps[map];
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Map '{0}' does not exist.", map);
			}
		}

		map::Map* World::get_active() const
		{
			return m_current;
		}

		const std::string& World::name() const
		{
			return m_project.getWorld().getName();
		}

		const std::string& World::file() const
		{
			return m_file;
		}

		bool World::loaded() const
		{
			return m_loaded;
		}

		ankerl::unordered_dense::map<std::string, map::Map>& World::maps()
		{
			return m_maps;
		}

	} // namespace map
} // namespace galaxy
