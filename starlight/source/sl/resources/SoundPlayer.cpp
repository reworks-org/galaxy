///
/// SoundPlayer.cpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "SoundPlayer.hpp"

namespace sl
{
	SoundPlayer::SoundPlayer(const std::string& script)
		:m_nullSound("")
	{
		// Load lua script from VFS.
		sol::state loader;

		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		loader.script(Locator::virtualFS->openAsString(fullPath));
		sol::table sounds = loader.get<sol::table>("sounds");

		if (!sounds.empty())
		{
			// Iterate over each sound effect in the lua table and construct it in place.
			sounds.for_each([this](std::pair<sol::object, sol::object> pair)
			{
				m_resourceMap.emplace(entt::HashedString(pair.first.as<const char*>()), pair.second.as<sol::table>());
			});
		}
		else
		{
			LOG_S(WARNING) << "Loaded empty sound table for script: " << script;
		}
	}

	SoundPlayer::~SoundPlayer()
	{
		// See clean().
		clean();
	}

	void SoundPlayer::changeVolume(const float volume)
	{
		// Correct volume range.
		float corrected = std::max(0.0f, volume);

		// Adjust volume of the sound effects.
		for (auto& it : m_resourceMap)
		{
			it.second.setVolume(corrected);
		}
	}

	void SoundPlayer::clean()
	{
		// Sound objects are wrapped in classes so destructors handle cleaning of object data.
		m_resourceMap.clear();
	}
}