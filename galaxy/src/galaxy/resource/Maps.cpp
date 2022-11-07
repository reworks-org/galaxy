///
/// Maps.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Maps.hpp"

namespace galaxy
{
	namespace resource
	{
		Maps::Maps() noexcept
			: m_folder {""}
		{
		}

		Maps::~Maps() noexcept
		{
		}

		void Maps::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto path = std::filesystem::path(file);
					if (path.extension() == ".tmj")
					{
						tson::Tileson tileson;
						auto map = tileson.parse(path);

						if (map->getStatus() == tson::ParseStatus::OK)
						{
							m_cache[path.stem().string()] = std::move(map);
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to parse '{0}' in tileson.", file);
						}
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no maps to load in '{0}'.", m_folder);
			}
		}
	} // namespace resource
} // namespace galaxy