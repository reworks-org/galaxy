///
/// MusicPlayer.cpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "MusicPlayer.hpp"

namespace sl
{
	MusicPlayer::MusicPlayer(const std::string& script)
		:m_nullMusic("")
	{
		// Load lua script from VFS.
		sol::state loader;

		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		loader.script(Locator::virtualFS->openAsString(fullPath));
		sol::table music = loader.get<sol::table>("music");

		if (!music.empty())
		{
			// Iterate over each sound effect in the lua table and construct it in place.
			music.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_resourceMap.emplace(entt::HashedString(pair.first.as<const char*>()), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Loaded empty music table for script: " << script;
		}
	}

	MusicPlayer::~MusicPlayer()
	{
		// See clean().
		clean();
	}

	void MusicPlayer::changeVolume(const float volume)
	{
		// Correct volume range.
		float corrected = std::max(0.0f, volume);

		// Adjust volume of the music.
		for (auto& it : m_resourceMap)
		{
			it.second.setVolume(corrected);
		}
	}

	void MusicPlayer::play(const std::string& music)
	{
		get(music.c_str()).play();
	}

	void MusicPlayer::stop(const std::string& music)
	{
		get(music.c_str()).stop();
	}

	void MusicPlayer::resume(const std::string& music)
	{
		get(music.c_str()).resume();
	}

	void MusicPlayer::clean()
	{
		// Music objects are wrapped in classes so destructors handle cleaning of object data.
		m_resourceMap.clear();
	}
}