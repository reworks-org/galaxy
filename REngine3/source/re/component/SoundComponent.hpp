//
//  SoundComponent.hpp
//  REngine3
//
//  Created by reworks on 19/09/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_SOUNDCOMPONENT_HPP_
#define RENGINE3_SOUNDCOMPONENT_HPP_

#include <unordered_map>

#include "re/audio/Sound.hpp"
#include "re/component/Component.hpp"

namespace re
{
	class SoundComponent : public Component
	{
	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Set up the component.
		*/
		SoundComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~SoundComponent() override;

		/*
		* IMPORTS: std::string name of lua script.
		* EXPORTS: none
		* PURPOSE: Add a sound to the component from the lua script.
		*/
		void AddSound(const std::string& script);

		/*
		* IMPORTS: id in std::string of sound.
		* EXPORTS: smart pointer to sound.
		* PURPOSE: Retrieve a sound from the component.
		*/
		std::shared_ptr<Sound> Get(const std::string& id);

	private:
		std::unordered_map<std::string, std::shared_ptr<Sound>> m_sounds;
	};
}

#endif