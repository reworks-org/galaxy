///
/// SoundPlayer.cpp
/// starlight
///
/// Created by reworks on 12/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/allegro_audio.h>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "SoundPlayer.hpp"

namespace sl
{
	SoundPlayer::SoundPlayer(const std::string& script)
	{
		sol::state lua;
		lua.script(Locator::m_virtualFS->openAsString(script));
		sol::table sound = lua.get<sol::table>("sound");

		sound.for_each([this](std::pair<sol::object, sol::object> pair)
		{
			m_resourceMap.emplace(pair.first.as<entt::HashedString>(), pair.second.as<sol::table>());
		});
	}

	SoundPlayer::~SoundPlayer()
	{
		clean();
	}

	void SoundPlayer::changeSoundVolume(float volume)
	{
		if (volume > 1.0)
		{
			volume = 1.0f;
		}

		if (volume < 0.0)
		{
			volume = 0.0f;
		}

		for (auto& it : m_resourceMap)
		{
			it.second.setVolume(volume);
		}
	}

	void SoundPlayer::clean()
	{
		m_resourceMap.clear();
	}
}