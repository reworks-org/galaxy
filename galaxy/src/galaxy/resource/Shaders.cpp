///
/// Shaders.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <ranges>

#include "galaxy/utils/StringUtils.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Shaders.hpp"

namespace galaxy
{
	namespace resource
	{
		void Shaders::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (auto& file : contents)
				{
					file = strutils::replace_first(file, GALAXY_VERTEX_EXT, "");
					file = strutils::replace_first(file, GALAXY_FRAGMENT_EXT, "");
				}

				auto unique_range = std::ranges::unique(contents);
				contents.erase(unique_range.begin(), unique_range.end());

				std::ranges::sort(contents);
				unique_range = std::ranges::unique(contents);
				contents.erase(unique_range.begin(), unique_range.end());

				for (const auto& file : contents)
				{
					const auto name = std::filesystem::path(file).stem().string();
					m_cache[name]   = std::make_shared<graphics::Shader>(file + GALAXY_VERTEX_EXT, file + GALAXY_FRAGMENT_EXT);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found to shaders to load in '{0}'.", m_folder);
			}
		}

		void Shaders::reload()
		{
			if (!m_folder.empty())
			{
				clear();
				load(m_folder);
			}
		}
	} // namespace resource
} // namespace galaxy