//
//  AudioSystem.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/entity/Entity.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"

#include "AudioSystem.hpp"

namespace re
{
	AudioSystem::~AudioSystem()
	{
		m_entitys.clear();
	}

	void AudioSystem::submit(World* world)
	{
		for (auto& it : world->getAliveEntitys())
		{
			if (it.second.has<MusicComponent>() || it.second.has<SoundComponent>())
			{
				addEntity(&it.second);
			}
		}
	}

	void AudioSystem::addEntity(Entity* e)
	{
		if (e->m_systemIds.find("AudioSystem") == e->m_systemIds.end())
		{
			e->m_systemIds.emplace("AudioSystem", std::type_index(typeid(AudioSystem)));
		}
		
		m_entitys.emplace(e->m_name, e);
	}

	void AudioSystem::removeEntity(const std::string& name)
	{
		auto found = m_entitys.find(name);

		if (found != m_entitys.end())
		{
			m_entitys.erase(name);
		}
	}

	sf::Sound* AudioSystem::getSound(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string sound = accessor.substr(lastdot + 1);

		return m_entitys[name]->get<SoundComponent>()->m_sounds[sound].second.get();
	}

	sf::Music* AudioSystem::getMusic(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string music = accessor.substr(lastdot + 1);

		return m_entitys[name]->get<MusicComponent>()->m_music[music].second.get();
	}

	void AudioSystem::setGlobalMusicVolume(float volume)
	{
		for (auto& e : m_entitys)
		{
			auto* map = &(e.second->get<MusicComponent>()->m_music);
			for (auto it = map->begin(); it != map->end(); it++)
			{
				auto m = it->second.second.get();
				float ov = m->getVolume();
				float nv = (volume + ov) / 2.0f;
				m->setVolume(nv);
			}
		}
	}

	void AudioSystem::setGlobalSoundVolume(float volume)
	{
		for (auto& e : m_entitys)
		{
			auto* map = &(e.second->get<SoundComponent>()->m_sounds);
			for (auto it = map->begin(); it != map->end(); it++)
			{
				auto s = it->second.second.get();
				float ov = s->getVolume();
				float nv = (volume + ov) / 2.0f;
				s->setVolume(nv);
			}
		}
	}

	void AudioSystem::clean()
	{
		m_entitys.clear();
	}
}
