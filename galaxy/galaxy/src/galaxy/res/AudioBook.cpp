///
/// AudioBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"

#include "AudioBook.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	AudioBook::AudioBook(const std::string& json) noexcept
	{
		createFromJSON(json);
	}

	AudioBook::~AudioBook() noexcept
	{
		clear();
	}

	void AudioBook::createFromJSON(const std::string& json)
	{
		auto path = std::filesystem::path(galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + json);

		std::ifstream is(path.string(), std::ifstream::in);
		if (is.good())
		{
			nlohmann::json j;
			is >> j;

			nlohmann::json arr = j.at("audiobook");
			std::for_each(arr.begin(), arr.end(), [this](const nlohmann::json& audio)
				{
					auto audioPath = std::filesystem::path(FileSystem::s_root + FileSystem::s_audio + audio.get<std::string>());
					this->add(audioPath.stem().string(), audioPath.string());
				});
		}

		is.close();
	}

	void AudioBook::clear() noexcept
	{
		m_resources.clear();
	}
}