///
/// Fonts.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Fonts.hpp"

namespace galaxy
{
	namespace resource
	{
		void Fonts::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto path     = std::filesystem::path(file);
					const auto path_str = path.stem().string();

					try
					{
						const auto _pos = path_str.find_last_of('_');
						const auto name = path_str.substr(0, _pos);
						const auto size = path_str.substr(_pos + 1, path_str.size() - _pos);

						if (!name.empty() && !size.empty())
						{
							m_cache[name] = std::make_shared<graphics::Font>(path.string(), std::stoi(size));
						}
						else
						{
							GALAXY_LOG(GALAXY_ERROR,
								"Failed to parse font file name, make sure format is 'Arial_11.ext'. File that failed was named '{0}'.",
								path_str);
						}
					}
					catch (const std::exception& e)
					{
						GALAXY_UNUSED(e);

						GALAXY_LOG(GALAXY_ERROR,
							"Failed to parse font file name, make sure format is 'Arial_11.ext'. File that failed was named '{0}'.",
							path_str);
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no fonts to load in '{0}'.", m_folder);
			}
		}

		void Fonts::reload()
		{
			if (!m_folder.empty())
			{
				clear();
				load(m_folder);
			}
		}
	} // namespace resource
} // namespace galaxy