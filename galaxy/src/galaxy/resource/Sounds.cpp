///
/// Sounds.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Sounds.hpp"

namespace galaxy
{
	namespace resource
	{
		Sounds::Sounds() noexcept
		{
		}

		Sounds::~Sounds() noexcept
		{
		}

		void Sounds::load_sfx(std::string_view folder)
		{
			m_sfx_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_sfx_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<audio::Sound>(audio::Sound::Type::SFX, static_cast<std::string>(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no sfx resources to load in '{0}'.", m_sfx_path);
			}
		}

		void Sounds::load_music(std::string_view folder)
		{
			m_music_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_music_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<audio::Sound>(audio::Sound::Type::MUSIC, static_cast<std::string>(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no music resources to load in '{0}'.", m_music_path);
			}
		}

		void Sounds::load_dialogue(std::string_view folder)
		{
			m_dialogue_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_dialogue_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<audio::Sound>(audio::Sound::Type::DIALOGUE, static_cast<std::string>(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no voice resources to load in '{0}'.", m_dialogue_path);
			}
		}
	} // namespace resource
} // namespace galaxy