///
/// Fonts.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "galaxy/embedded/RobotoLight.hpp"

#include "Fonts.hpp"

namespace galaxy
{
	namespace resource
	{
		Fonts::Fonts()
		{
		}

		Fonts::~Fonts()
		{
		}

		std::future<void> Fonts::load(std::string_view folder)
		{
			clear();

			m_folder = folder;

			return core::ServiceLocator<BS::thread_pool>::ref().submit([&]() {
				if (!m_folder.empty())
				{
					auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

					// Load default font(s) first, incase of override.
					m_cache["RobotoLight"] = std::make_shared<graphics::Font>(embedded::roboto_light, embedded::roboto_light_len);

					auto contents = fs.list_directory(m_folder);
					if (!contents.empty())
					{
						for (const auto& file : contents)
						{
							const auto path = std::filesystem::path(file);
							const auto name = path.stem().string();

							m_cache[name] = std::make_shared<graphics::Font>(path.string());
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Found no fonts to load in '{0}'.", m_folder);
					}
				}
			});
		}

		void Fonts::build()
		{
			for (auto&& [key, font] : m_cache)
			{
				font->build();
			}
		}
	} // namespace resource
} // namespace galaxy