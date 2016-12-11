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
		
		m_entitys.emplace(e->m_name, e);
	}

	void AudioSystem::RemoveEntity(const std::string& name)
	{
		auto found = m_entitys.find(name);

		if (found != m_entitys.end())
		{
			m_entitys.erase(name);
		}
	}

	sf::Sound* AudioSystem::GetSound(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string sound = accessor.substr(lastdot + 1);

		return m_entitys[name]->Get<SoundComponent>()->m_sounds[sound].second.get();
	}

	sf::Music* AudioSystem::GetMusic(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string music = accessor.substr(lastdot + 1);

		return m_entitys[name]->Get<MusicComponent>()->m_music[music].second.get();
	}

	void AudioSystem::SetGlobalMusicVolume(float volume)
	{
		for (auto& e : m_entitys)
		{
			auto* map = &(e.second->Get<MusicComponent>()->m_music);
			for (auto it = map->begin(); it != map->end(); it++)
			{
				auto m = it->second.second.get();
				float ov = m->getVolume();
				float nv = (volume + ov) / 2.0f;
				m->setVolume(nv);
			}
		}
	}

	void AudioSystem::SetGlobalSoundVolume(float volume)
	{
		for (auto& e : m_entitys)
		{
			auto* map = &(e.second->Get<SoundComponent>()->m_sounds);
			for (auto it = map->begin(); it != map->end(); it++)
			{
				auto s = it->second.second.get();
				float ov = s->getVolume();
				float nv = (volume + ov) / 2.0f;
				s->setVolume(nv);
			}
		}
	}

	void AudioSystem::Clean()
	{
		m_entitys.clear();
	}
}