///
/// Materials.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Materials.hpp"

namespace galaxy
{
	namespace resource
	{
		Materials::Materials()
			: m_folder {""}
		{
		}

		Materials::~Materials()
		{
		}

		void Materials::load(std::string_view folder)
		{
			m_folder = std::string(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto data = fs.open(file);
					if (!data.empty())
					{
						const auto name = std::filesystem::path(file).stem().string();
						m_cache[name]   = std::make_shared<physics::Material>(nlohmann::json::parse(file));
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no physics materials to load in '{0}'.", m_folder);
			}
		}
	} // namespace resource
} // namespace galaxy