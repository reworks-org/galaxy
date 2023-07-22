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
		Sounds::Sounds()
		{
		}

		Sounds::~Sounds()
		{
		}

		std::future<void> Sounds::load(std::string_view folder)
		{
			clear();

			m_stem = folder;

			return core::ServiceLocator<BS::thread_pool>::ref().submit([&]() {
				if (!m_stem.empty())
				{
					load_sfx(m_stem + "sfx/");
					load_music(m_stem + "music/");
					load_dialogue(m_stem + "dialogue/");
				}
			});
		}

		void Sounds::load_sfx(std::string_view folder)
		{
			m_sfx_path = folder;

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_sfx_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<media::Sound>(media::Sound::Type::SFX, std::string(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no sfx resources to load in '{0}'.", m_sfx_path);
			}
		}

		void Sounds::load_music(std::string_view folder)
		{
			m_music_path = folder;

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_music_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<media::Sound>(media::Sound::Type::MUSIC, std::string(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no music resources to load in '{0}'.", m_music_path);
			}
		}

		void Sounds::load_dialogue(std::string_view folder)
		{
			m_dialogue_path = folder;

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_dialogue_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();
					m_cache[key]   = std::make_shared<media::Sound>(media::Sound::Type::DIALOGUE, std::string(file));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no voice resources to load in '{0}'.", m_dialogue_path);
			}
		}
	} // namespace resource
} // namespace galaxy