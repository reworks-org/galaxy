///
/// Media.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Media.hpp"

namespace galaxy
{
	namespace resource
	{
		std::shared_ptr<media::Sound> SFXLoader::operator()(const std::string& file)
		{
			return std::make_shared<media::Sound>(media::SoundType::SFX, file);
		}

		std::shared_ptr<media::Sound> MusicLoader::operator()(const std::string& file)
		{
			return std::make_shared<media::Sound>(media::SoundType::MUSIC, file);
		}

		std::shared_ptr<media::Sound> DialogueLoader::operator()(const std::string& file)
		{
			return std::make_shared<media::Sound>(media::SoundType::DIALOGUE, file);
		}

		void VideoLoader::build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<media::Video>>& cache)
		{
			for (auto&& [key, video] : cache)
			{
				video->build();
			}
		}

		std::shared_ptr<media::Video> VideoLoader::operator()(const std::string& file)
		{
			auto ptr = std::make_shared<media::Video>();
			ptr->load(file);

			return ptr;
		}
	} // namespace resource
} // namespace galaxy
