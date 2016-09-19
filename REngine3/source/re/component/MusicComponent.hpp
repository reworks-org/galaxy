//
//  MusicComponent.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_MUSICCOMPONENT_HPP_
#define RENGINE3_MUSICCOMPONENT_HPP_

#include <unordered_map>

#include "re/audio/Music.hpp"
#include "re/component/Component.hpp"

namespace re
{
	class MusicComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		MusicComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~MusicComponent() override;

		/*
		* IMPORTS: std::string name of lua script.
		* EXPORTS: none
		* PURPOSE: Add a sound to the component from the lua script.
		*/
		void AddMusic(const std::string& script);

		/*
		* IMPORTS: id in std::string of sound.
		* EXPORTS: smart pointer to sound.
		* PURPOSE: Retrieve a sound from the component.
		*/
		std::shared_ptr<Music> Get(const std::string& id);

	private:
		std::unordered_map<std::string, std::shared_ptr<Music>> m_music;
	};
}

#endif