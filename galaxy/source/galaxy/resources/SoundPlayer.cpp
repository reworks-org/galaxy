///
/// SoundPlayer.cpp
/// galaxy
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "SoundPlayer.hpp"

galaxy
{
	SoundPlayer::SoundPlayer(const std::string& script)
		:m_nullSound("")
	{
		std::string fullPath = Locator::world->m_scriptFolderPath + script;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));
		sol::table sounds = Locator::lua->get<sol::table>("sounds");

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