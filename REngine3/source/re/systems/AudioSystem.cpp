//
//  AudioSystem.cpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/app/World.hpp"
#include "re/entity/Entity.hpp"
#include "re/services/ServiceLocator.hpp"
#include "re/component/SoundComponent.hpp"
#include "re/component/MusicComponent.hpp"

#include "AudioSystem.hpp"

namespace re
{
	AudioSystem::AudioSystem(int defaultEffectVolume, int defaultMusicVolume)
	:m_typeAsString("AudioSystem"), m_effectVolume(defaultEffectVolume), m_musicVolume(defaultMusicVolume)
	{
	}

	AudioSystem::~AudioSystem()
	{
		m_entitys.clear();
	}

	void AudioSystem::prepareAudio()
	{
		for (auto& e : Locator::get<World>()->getAliveEntitys())
		{
			if (e.second.has<MusicComponent>())
			{
				auto* map = &(e.second.get<MusicComponent>()->m_music);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second->setVolume(m_musicVolume);
				}
			}

			if (e.second.has<SoundComponent>())
			{
				auto* map = &(e.second.get<SoundComponent>()->m_sounds);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second.second->setVolume(m_effectVolume);
				}
			}
		}

		for (auto& e : Locator::get<World>()->getDeadEntitys())
		{
			if (e.second.has<MusicComponent>())
			{
				auto* map = &(e.second.get<MusicComponent>()->m_music);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second->setVolume(m_musicVolume);
				}
			}

			if (e.second.has<SoundComponent>())
			{
				auto* map = &(e.second.get<SoundComponent>()->m_sounds);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second.second->setVolume(m_effectVolume);
				}
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
			Entity* e = m_entitys[name];

			if (e->has<MusicComponent>())
			{
				auto* map = &(e->get<MusicComponent>()->m_music);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second->stop();
				}
			}
			
			m_entitys.erase(name);
		}
	}

	sf::Sound& AudioSystem::getSound(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string sound = accessor.substr(lastdot + 1);

		return *(m_entitys[name]->get<SoundComponent>()->m_sounds[sound].second);
	}

	sf::Music& AudioSystem::getMusic(const std::string& accessor)
	{
		auto lastdot = accessor.find_last_of(".");
		std::string name = accessor.substr(0, lastdot);
		std::string music = accessor.substr(lastdot + 1);

		return *(m_entitys[name]->get<MusicComponent>()->m_music[music]);
	}

	void AudioSystem::setMusicVolume(int newVolume)
	{
		m_musicVolume = newVolume;

		for (auto& e : m_entitys)
		{
			if (e.second->has<MusicComponent>())
			{
				auto* map = &(e.second->get<MusicComponent>()->m_music);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second->setVolume(m_musicVolume);
				}
			}
		}
	}

	void AudioSystem::setEffectVolume(int newVolume)
	{
		m_effectVolume = newVolume;

		for (auto& e : m_entitys)
		{
			if (e.second->has<SoundComponent>())
			{
				auto* map = &(e.second->get<SoundComponent>()->m_sounds);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second.second->setVolume(m_effectVolume);
				}
			}
		}
	}

	int AudioSystem::getEffectVolume() const
	{
		return m_effectVolume;
	}

	int AudioSystem::getMusicVolume() const
	{
		return m_musicVolume;
	}

	void AudioSystem::stop()
	{
		for (auto& e : m_entitys)
		{
			if (e.second->has<MusicComponent>())
			{
				auto* map = &(e.second->get<MusicComponent>()->m_music);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second->stop();
				}
			}

			if (e.second->has<SoundComponent>())
			{
				auto* map = &(e.second->get<SoundComponent>()->m_sounds);
				for (auto it = map->begin(); it != map->end(); it++)
				{
					it->second.second->stop();
				}
			}
		}
	}

	void AudioSystem::clean()
	{
		stop();

		m_entitys.clear();
	}
}
