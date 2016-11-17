//
//  AudioSystem.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"

#include "AudioSystem.hpp"

namespace re
{
	AudioSystem::~AudioSystem()
	{
		m_entitys.clear();
		m_music.clear();
		m_sounds.clear();
	}

	void AudioSystem::AutoSubmit(World* world)
	{
		for (auto& it : world->GetAlive())
		{
			if (it.second.Has<MusicComponent>() || it.second.Has<SoundComponent>())
			{
				AddEntity(&it.second);
			}
		}
	}

	void AudioSystem::AddEntity(Entity* e)
	{
		e->m_systemIds.emplace("AudioSystem", std::type_index(typeid(AudioSystem)));
		if (e->Has<MusicComponent>())
		{
			m_music.emplace(e->m_name, &e->Get<MusicComponent>()->m_music);
		}

		if (e->Has<SoundComponent>())
		{
			m_music.emplace(e->m_name, &e->Get<SoundComponent>()->m_sounds);
		}
	}

	void AudioSystem::RemoveEntity(const std::string& name)
	{
		auto foundA = m_music.find(name);
		auto foundB = m_sounds.find(name);

		if (foundA != m_music.end())
		{
			m_music.erase(name);
		}

		if (foundB != m_sounds.end())
		{
			m_sounds.erase(name);
		}
	}

	std::shared_ptr<Sound> AudioSystem::GetSound(const std::string& accessor)
	{
		size_t lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string sound = accessor.substr(lastdot);

		return m_sounds.at(name)->at(sound);
	}

	std::shared_ptr<Music> AudioSystem::GetMusic(const std::string& accessor)
	{
		size_t lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string music = accessor.substr(lastdot);

		return m_music.at(name)->at(music);
	}

	void AudioSystem::Clean()
	{
		m_entitys.clear();
		m_music.clear();
		m_sounds.clear();
	}
}