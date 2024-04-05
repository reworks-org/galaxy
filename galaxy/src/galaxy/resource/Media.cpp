///
/// Media.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Media.hpp"

namespace galaxy
{
	namespace resource
	{
		std::unique_ptr<media::Sound> SoundLoader::operator()(const std::string& file)
		{
			auto resource = std::make_unique<media::Sound>();
			if (!resource->load(media::SoundType::SFX, file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load resource '{0}'.", file);
				return nullptr;
			}

			return resource;
		}

		std::unique_ptr<media::Sound> MusicLoader::operator()(const std::string& file)
		{
			auto resource = std::make_unique<media::Sound>();
			if (!resource->load(media::SoundType::MUSIC, file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load resource '{0}'.", file);
				return nullptr;
			}

			return resource;
		}

		std::unique_ptr<media::Sound> VoiceLoader::operator()(const std::string& file)
		{
			auto resource = std::make_unique<media::Sound>();
			if (!resource->load(media::SoundType::VOICE, file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load resource '{0}'.", file);
				return nullptr;
			}

			return resource;
		}
	} // namespace resource
} // namespace galaxy
