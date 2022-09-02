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
		Fonts::Fonts() noexcept
			: m_folder {""}
		{
		}

		Fonts::~Fonts() noexcept
		{
		}

		void Fonts::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

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

		void Fonts::reload()
		{
			if (!m_folder.empty())
			{
				clear();
				load(m_folder);
				build();
			}
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