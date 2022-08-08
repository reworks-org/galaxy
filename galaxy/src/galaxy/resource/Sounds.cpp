///
/// Sounds.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/audio/AudioEngine.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Sounds.hpp"

namespace galaxy
{
	namespace resource
	{
		void Sounds::load_sfx(std::string_view folder)
		{
			m_sfx_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();

			auto contents = fs.list_directory(m_sfx_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();

					m_cache[key] = ae.make_sfx(static_cast<std::string>(file), 1.0f);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to load any sfx resources from '{0}'.", m_sfx_path);
			}
		}

		void Sounds::load_music(std::string_view folder)
		{
			m_music_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();

			auto contents = fs.list_directory(m_music_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();

					m_cache[key] = ae.make_music(static_cast<std::string>(file), 1.0f);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to load any music resources from '{0}'.", m_music_path);
			}
		}

		void Sounds::load_dialogue(std::string_view folder)
		{
			m_voice_path = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& ae = core::ServiceLocator<audio::AudioEngine>::ref();

			auto contents = fs.list_directory(m_voice_path);
			if (!contents.empty())
			{
				for (const auto& file : contents)
				{
					const auto key = std::filesystem::path(file).stem().string();

					m_cache[key] = ae.make_voice(static_cast<std::string>(file), 1.0f);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to load any voiced resources from '{0}'.", m_voice_path);
			}
		}

		void Sounds::reload()
		{
			if (!m_sfx_path.empty() && !m_music_path.empty() && !m_voice_path.empty())
			{
				clear();

				load_sfx(m_sfx_path);
				load_music(m_music_path);
				load_dialogue(m_voice_path);
			}
		}
	} // namespace resource
} // namespace galaxy