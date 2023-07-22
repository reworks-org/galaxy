///
/// Scripts.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Scripts.hpp"

namespace galaxy
{
	namespace resource
	{
		Scripts::Scripts()
		{
		}

		Scripts::~Scripts()
		{
		}

		std::future<void> Scripts::load(std::string_view folder)
		{
			clear();

			m_folder = folder;

			return core::ServiceLocator<BS::thread_pool>::ref().submit([&]() {
				if (!m_folder.empty())
				{
					auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

					auto contents = fs.list_directory(m_folder);
					if (!contents.empty())
					{
						for (const auto& file : contents)
						{
							const auto name = std::filesystem::path(file).stem().string();
							m_cache[name]   = std::make_shared<lua::BasicScript>(file);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Found no scripts to load in '{0}'.", m_folder);
					}
				}
			});
		}
	} // namespace resource
} // namespace galaxy